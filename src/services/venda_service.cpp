#include "services/venda_service.h"
#include "database/database_manager.h"
#include <algorithm>
#include <numeric>
#include <map>

VendaService::VendaService(DatabaseManager* db_manager) 
    : db_manager(db_manager) {
}

bool VendaService::registrarVenda(Venda& venda) {
    return processarVenda(venda);
}

std::unique_ptr<Venda> VendaService::buscarVenda(int id) {
    return db_manager->buscarVenda(id);
}

std::vector<Venda> VendaService::listarVendas() {
    return db_manager->listarVendas();
}

std::vector<Venda> VendaService::listarVendasPorPeriodo(const QDate& inicio, const QDate& fim) {
    return db_manager->listarVendasPorPeriodo(inicio, fim);
}

bool VendaService::cancelarVenda(int id) {
    if (!vendaPodeSerCancelada(id)) {
        return false;
    }
    
    // Estornar estoque antes de cancelar
    estornarEstoque(id);
    
    return db_manager->cancelarVenda(id);
}

bool VendaService::processarVenda(Venda& venda) {
    // Validar venda
    if (!validarVenda(venda)) {
        return false;
    }
    
    // Verificar estoque
    if (!validarEstoque(venda)) {
        return false;
    }
    
    // Registrar venda no banco
    if (!db_manager->inserirVenda(venda)) {
        return false;
    }
    
    // Atualizar estoque
    return atualizarEstoqueVenda(venda);
}

bool VendaService::validarEstoque(const Venda& venda) {
    for (const auto& item : venda.getItens()) {
        auto produto = db_manager->buscarProduto(item.produto_id);
        if (!produto) {
            return false;
        }
        
        if (produto->getQuantidadeEstoque() < item.quantidade) {
            return false;
        }
    }
    return true;
}

bool VendaService::atualizarEstoqueVenda(const Venda& venda) {
    for (const auto& item : venda.getItens()) {
        if (!db_manager->removerEstoque(item.produto_id, item.quantidade)) {
            return false;
        }
    }
    return true;
}

bool VendaService::estornarEstoque(int venda_id) {
    auto venda = buscarVenda(venda_id);
    if (!venda) {
        return false;
    }
    
    for (const auto& item : venda->getItens()) {
        if (!db_manager->adicionarEstoque(item.produto_id, item.quantidade)) {
            return false;
        }
    }
    return true;
}

std::vector<Venda> VendaService::buscarVendasPorCliente(int cliente_id) {
    auto vendas = listarVendas();
    std::vector<Venda> resultado;
    
    std::copy_if(vendas.begin(), vendas.end(),
                std::back_inserter(resultado),
                [cliente_id](const Venda& v) {
                    return v.getClienteId() == cliente_id && !v.isCancelada();
                });
    
    return resultado;
}

std::vector<Venda> VendaService::buscarVendasPorFormaPagamento(FormaPagamento forma) {
    auto vendas = listarVendas();
    std::vector<Venda> resultado;
    
    std::copy_if(vendas.begin(), vendas.end(),
                std::back_inserter(resultado),
                [forma](const Venda& v) {
                    return v.getFormaPagamento() == forma && !v.isCancelada();
                });
    
    return resultado;
}

std::vector<Venda> VendaService::buscarVendasCanceladas() {
    auto vendas = listarVendas();
    std::vector<Venda> resultado;
    
    std::copy_if(vendas.begin(), vendas.end(),
                std::back_inserter(resultado),
                [](const Venda& v) {
                    return v.isCancelada();
                });
    
    return resultado;
}

double VendaService::calcularTotalVendasDia(const QDate& data) {
    auto vendas = listarVendasPorPeriodo(data, data);
    double total = 0.0;
    
    for (const auto& venda : vendas) {
        if (!venda.isCancelada()) {
            total += venda.getTotal();
        }
    }
    
    return total;
}

double VendaService::calcularTotalVendasMes(int mes, int ano) {
    QDate inicio(ano, mes, 1);
    QDate fim = inicio.addMonths(1).addDays(-1);
    
    auto vendas = listarVendasPorPeriodo(inicio, fim);
    double total = 0.0;
    
    for (const auto& venda : vendas) {
        if (!venda.isCancelada()) {
            total += venda.getTotal();
        }
    }
    
    return total;
}

double VendaService::calcularTotalVendasAno(int ano) {
    QDate inicio(ano, 1, 1);
    QDate fim(ano, 12, 31);
    
    auto vendas = listarVendasPorPeriodo(inicio, fim);
    double total = 0.0;
    
    for (const auto& venda : vendas) {
        if (!venda.isCancelada()) {
            total += venda.getTotal();
        }
    }
    
    return total;
}

int VendaService::contarVendasDia(const QDate& data) {
    auto vendas = listarVendasPorPeriodo(data, data);
    return std::count_if(vendas.begin(), vendas.end(),
                        [](const Venda& v) { return !v.isCancelada(); });
}

int VendaService::contarVendasMes(int mes, int ano) {
    QDate inicio(ano, mes, 1);
    QDate fim = inicio.addMonths(1).addDays(-1);
    
    auto vendas = listarVendasPorPeriodo(inicio, fim);
    return std::count_if(vendas.begin(), vendas.end(),
                        [](const Venda& v) { return !v.isCancelada(); });
}

bool VendaService::validarVenda(const Venda& venda) {
    return venda.isValida();
}

std::string VendaService::obterErrosValidacao(const Venda& venda) {
    return venda.validar();
}

std::vector<VendaService::ResumoVendas> VendaService::resumoVendasPorDia(const QDate& inicio, const QDate& fim) {
    std::vector<ResumoVendas> resumos;
    
    for (QDate data = inicio; data <= fim; data = data.addDays(1)) {
        ResumoVendas resumo;
        resumo.data = data;
        resumo.quantidade_vendas = contarVendasDia(data);
        resumo.total_vendas = calcularTotalVendasDia(data);
        resumo.ticket_medio = (resumo.quantidade_vendas > 0) ? 
                             resumo.total_vendas / resumo.quantidade_vendas : 0.0;
        
        resumos.push_back(resumo);
    }
    
    return resumos;
}

std::vector<VendaService::VendaPorFormaPagamento> VendaService::vendasPorFormaPagamento(const QDate& inicio, const QDate& fim) {
    auto vendas = listarVendasPorPeriodo(inicio, fim);
    std::map<FormaPagamento, std::pair<int, double>> dados;
    
    double total_geral = 0.0;
    
    for (const auto& venda : vendas) {
        if (!venda.isCancelada()) {
            FormaPagamento forma = venda.getFormaPagamento();
            dados[forma].first++; // quantidade
            dados[forma].second += venda.getTotal(); // total
            total_geral += venda.getTotal();
        }
    }
    
    std::vector<VendaPorFormaPagamento> resultado;
    for (const auto& par : dados) {
        VendaPorFormaPagamento item;
        item.forma = par.first;
        item.quantidade = par.second.first;
        item.total = par.second.second;
        item.percentual = (total_geral > 0) ? (item.total / total_geral) * 100.0 : 0.0;
        resultado.push_back(item);
    }
    
    return resultado;
}

double VendaService::calcularTicketMedio(const QDate& inicio, const QDate& fim) {
    auto vendas = listarVendasPorPeriodo(inicio, fim);
    
    std::vector<double> totais;
    for (const auto& venda : vendas) {
        if (!venda.isCancelada()) {
            totais.push_back(venda.getTotal());
        }
    }
    
    if (totais.empty()) {
        return 0.0;
    }
    
    double soma = std::accumulate(totais.begin(), totais.end(), 0.0);
    return soma / totais.size();
}

FormaPagamento VendaService::formaPagamentoMaisUsada(const QDate& inicio, const QDate& fim) {
    auto dados = vendasPorFormaPagamento(inicio, fim);
    
    if (dados.empty()) {
        return FormaPagamento::DINHEIRO;
    }
    
    auto max_item = std::max_element(dados.begin(), dados.end(),
        [](const VendaPorFormaPagamento& a, const VendaPorFormaPagamento& b) {
            return a.quantidade < b.quantidade;
        });
    
    return max_item->forma;
}

QDate VendaService::diaMaiorVenda(const QDate& inicio, const QDate& fim) {
    auto resumos = resumoVendasPorDia(inicio, fim);
    
    if (resumos.empty()) {
        return QDate();
    }
    
    auto max_resumo = std::max_element(resumos.begin(), resumos.end(),
        [](const ResumoVendas& a, const ResumoVendas& b) {
            return a.total_vendas < b.total_vendas;
        });
    
    return max_resumo->data;
}

bool VendaService::vendaExiste(int id) {
    return buscarVenda(id) != nullptr;
}

bool VendaService::vendaPodeSerCancelada(int id) {
    auto venda = buscarVenda(id);
    return venda && !venda->isCancelada();
}
