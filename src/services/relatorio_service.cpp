#include "services/relatorio_service.h"
#include "database/database_manager.h"
#include <QLocale>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <numeric>
#include <algorithm>

RelatorioService::RelatorioService(DatabaseManager* db_manager) 
    : db_manager(db_manager) {
}

std::vector<RelatorioService::RelatorioVendas> RelatorioService::relatorioVendasDiarias(const QDate& inicio, const QDate& fim) {
    auto dados_db = db_manager->relatorioVendasDiarias(inicio, fim);
    std::vector<RelatorioVendas> resultado;
    
    for (const auto& item : dados_db) {
        RelatorioVendas relatorio;
        relatorio.data = item.data;
        relatorio.quantidade_vendas = item.quantidade_vendas;
        relatorio.total_vendas = item.total_vendas;
        relatorio.quantidade_itens = item.quantidade_itens;
        relatorio.ticket_medio = (item.quantidade_vendas > 0) ? 
                                item.total_vendas / item.quantidade_vendas : 0.0;
        resultado.push_back(relatorio);
    }
    
    return resultado;
}

std::vector<RelatorioService::RelatorioVendas> RelatorioService::relatorioVendasMensais(int ano) {
    auto dados_db = db_manager->relatorioVendasMensais(ano);
    std::vector<RelatorioVendas> resultado;
    
    for (const auto& item : dados_db) {
        RelatorioVendas relatorio;
        relatorio.data = item.data;
        relatorio.quantidade_vendas = item.quantidade_vendas;
        relatorio.total_vendas = item.total_vendas;
        relatorio.quantidade_itens = item.quantidade_itens;
        relatorio.ticket_medio = (item.quantidade_vendas > 0) ? 
                                item.total_vendas / item.quantidade_vendas : 0.0;
        resultado.push_back(relatorio);
    }
    
    return resultado;
}

RelatorioService::RelatorioFinanceiro RelatorioService::relatorioFinanceiroPeriodo(const QDate& inicio, const QDate& fim) {
    RelatorioFinanceiro relatorio;
    relatorio.periodo_inicio = inicio;
    relatorio.periodo_fim = fim;
    
    relatorio.total_vendas = db_manager->totalVendasPeriodo(inicio, fim);
    relatorio.quantidade_vendas = db_manager->quantidadeVendasPeriodo(inicio, fim);
    
    // Cálculos básicos
    relatorio.ticket_medio = (relatorio.quantidade_vendas > 0) ?
                            relatorio.total_vendas / relatorio.quantidade_vendas : 0.0;
    
    // Por enquanto, assumir custos como 70% das vendas (implementar cálculo real depois)
    relatorio.total_custos = relatorio.total_vendas * 0.7;
    relatorio.lucro_bruto = relatorio.total_vendas - relatorio.total_custos;
    relatorio.margem_lucro = (relatorio.total_vendas > 0) ?
                            (relatorio.lucro_bruto / relatorio.total_vendas) * 100.0 : 0.0;
    
    return relatorio;
}

std::vector<RelatorioService::ProdutoMaisVendido> RelatorioService::produtosMaisVendidos(const QDate& inicio, const QDate& fim, int limite) {
    auto dados_db = db_manager->produtosMaisVendidos(inicio, fim, limite);
    std::vector<ProdutoMaisVendido> resultado;
    
    double total_vendas = db_manager->totalVendasPeriodo(inicio, fim);
    
    for (const auto& item : dados_db) {
        ProdutoMaisVendido produto;
        produto.produto_id = item.produto_id;
        produto.nome = item.nome;
        produto.quantidade_vendida = item.quantidade_vendida;
        produto.total_vendido = item.total_vendido;
        produto.percentual_vendas = (total_vendas > 0) ?
                                   (item.total_vendido / total_vendas) * 100.0 : 0.0;
        
        // Buscar dados completos do produto
        auto prod = db_manager->buscarProduto(item.produto_id);
        if (prod) {
            produto.codigo = prod->getCodigo();
            produto.categoria = prod->getCategoria();
        }
        
        resultado.push_back(produto);
    }
    
    return resultado;
}

std::vector<RelatorioService::ProdutoMaisVendido> RelatorioService::produtosMenosVendidos(const QDate& inicio, const QDate& fim, int limite) {
    auto mais_vendidos = produtosMaisVendidos(inicio, fim, 1000); // Buscar todos
    
    // Ordenar do menor para o maior
    std::sort(mais_vendidos.begin(), mais_vendidos.end(),
             [](const ProdutoMaisVendido& a, const ProdutoMaisVendido& b) {
                 return a.quantidade_vendida < b.quantidade_vendida;
             });
    
    // Retornar apenas os primeiros 'limite' itens
    if (mais_vendidos.size() > static_cast<size_t>(limite)) {
        mais_vendidos.resize(limite);
    }
    
    return mais_vendidos;
}

std::vector<RelatorioService::RelatorioEstoque> RelatorioService::relatorioEstoque() {
    auto produtos = db_manager->listarProdutos(true); // apenas ativos
    std::vector<RelatorioEstoque> resultado;
    
    for (const auto& produto : produtos) {
        RelatorioEstoque item;
        item.produto_id = produto.getId();
        item.codigo = produto.getCodigo();
        item.nome = produto.getNome();
        item.categoria = produto.getCategoria();
        item.quantidade_atual = produto.getQuantidadeEstoque();
        item.estoque_minimo = produto.getEstoqueMinimo();
        item.valor_unitario = produto.getPreco();
        item.valor_total = produto.calcularValorEstoque();
        
        // Determinar status
        if (produto.getQuantidadeEstoque() == 0) {
            item.status = "SEM ESTOQUE";
        } else if (produto.precisaReposicao()) {
            item.status = "ESTOQUE BAIXO";
        } else {
            item.status = "OK";
        }
        
        resultado.push_back(item);
    }
    
    return resultado;
}

std::vector<RelatorioService::RelatorioEstoque> RelatorioService::relatorioEstoqueBaixo() {
    auto todos = relatorioEstoque();
    std::vector<RelatorioEstoque> resultado;
    
    std::copy_if(todos.begin(), todos.end(),
                std::back_inserter(resultado),
                [](const RelatorioEstoque& item) {
                    return item.status != "OK";
                });
    
    return resultado;
}

std::vector<RelatorioService::RelatorioEstoque> RelatorioService::relatorioEstoquePorCategoria(const std::string& categoria) {
    auto todos = relatorioEstoque();
    std::vector<RelatorioEstoque> resultado;
    
    std::copy_if(todos.begin(), todos.end(),
                std::back_inserter(resultado),
                [&categoria](const RelatorioEstoque& item) {
                    return item.categoria == categoria;
                });
    
    return resultado;
}

RelatorioService::AnaliseVendas RelatorioService::analisarVendas(const QDate& inicio, const QDate& fim) {
    AnaliseVendas analise;
    
    // Período de comparação (mesmo período do ano anterior)
    QDate inicio_comparacao = inicio.addYears(-1);
    QDate fim_comparacao = fim.addYears(-1);
    
    double vendas_atual = db_manager->totalVendasPeriodo(inicio, fim);
    double vendas_anterior = db_manager->totalVendasPeriodo(inicio_comparacao, fim_comparacao);
    
    int qtd_atual = db_manager->quantidadeVendasPeriodo(inicio, fim);
    int qtd_anterior = db_manager->quantidadeVendasPeriodo(inicio_comparacao, fim_comparacao);
    
    // Calcular crescimentos
    analise.crescimento_vendas = (vendas_anterior > 0) ?
                                ((vendas_atual - vendas_anterior) / vendas_anterior) * 100.0 : 0.0;
    
    analise.crescimento_quantidade = (qtd_anterior > 0) ?
                                    ((qtd_atual - qtd_anterior) / static_cast<double>(qtd_anterior)) * 100.0 : 0.0;
    
    // Encontrar dias de maior e menor venda
    auto vendas_diarias = relatorioVendasDiarias(inicio, fim);
    if (!vendas_diarias.empty()) {
        auto max_dia = std::max_element(vendas_diarias.begin(), vendas_diarias.end(),
                                       [](const RelatorioVendas& a, const RelatorioVendas& b) {
                                           return a.total_vendas < b.total_vendas;
                                       });
        
        auto min_dia = std::min_element(vendas_diarias.begin(), vendas_diarias.end(),
                                       [](const RelatorioVendas& a, const RelatorioVendas& b) {
                                           return a.total_vendas < b.total_vendas;
                                       });
        
        analise.dia_maior_venda = max_dia->data;
        analise.dia_menor_venda = min_dia->data;
    }
    
    // Forma de pagamento preferida (simplificado)
    analise.forma_pagamento_preferida = "Dinheiro"; // Implementar busca real
    
    // Percentual de cancelamentos (simplificado)
    analise.percentual_cancelamentos = 2.5; // Implementar cálculo real
    
    return analise;
}

RelatorioService::KPIs RelatorioService::obterKPIs() {
    KPIs kpis;
    
    QDate hoje = QDate::currentDate();
    QDate ontem = hoje.addDays(-1);
    QDate inicio_mes = QDate(hoje.year(), hoje.month(), 1);
    QDate inicio_mes_anterior = inicio_mes.addMonths(-1);
    QDate fim_mes_anterior = inicio_mes.addDays(-1);
    
    // Vendas
    kpis.vendas_hoje = db_manager->totalVendasPeriodo(hoje, hoje);
    kpis.vendas_ontem = db_manager->totalVendasPeriodo(ontem, ontem);
    kpis.vendas_mes_atual = db_manager->totalVendasPeriodo(inicio_mes, hoje);
    kpis.vendas_mes_anterior = db_manager->totalVendasPeriodo(inicio_mes_anterior, fim_mes_anterior);
    
    // Estoque
    auto produtos_estoque_baixo = db_manager->listarProdutosEstoqueBaixo();
    kpis.produtos_estoque_baixo = static_cast<int>(produtos_estoque_baixo.size());
    
    auto produtos = db_manager->listarProdutos(true);
    kpis.total_produtos_ativos = static_cast<int>(produtos.size());
    
    // Contar produtos sem estoque
    kpis.produtos_sem_estoque = static_cast<int>(
        std::count_if(produtos.begin(), produtos.end(),
                     [](const Produto& p) { return p.getQuantidadeEstoque() == 0; })
    );
    
    // Valor total do estoque
    kpis.valor_total_estoque = 0.0;
    for (const auto& produto : produtos) {
        kpis.valor_total_estoque += produto.calcularValorEstoque();
    }
    
    // Ticket médio do mês
    int vendas_mes = db_manager->quantidadeVendasPeriodo(inicio_mes, hoje);
    kpis.ticket_medio_mes = (vendas_mes > 0) ? kpis.vendas_mes_atual / vendas_mes : 0.0;
    
    return kpis;
}

std::string RelatorioService::gerarRelatorioVendasCSV(const QDate& inicio, const QDate& fim) {
    auto vendas = relatorioVendasDiarias(inicio, fim);
    std::stringstream csv;
    
    // Cabeçalho
    csv << "Data,Quantidade Vendas,Total Vendas,Ticket Médio\n";
    
    // Dados
    for (const auto& venda : vendas) {
        csv << formatarData(venda.data) << ","
            << venda.quantidade_vendas << ","
            << std::fixed << std::setprecision(2) << venda.total_vendas << ","
            << std::fixed << std::setprecision(2) << venda.ticket_medio << "\n";
    }
    
    return csv.str();
}

std::string RelatorioService::gerarRelatorioEstoqueCSV() {
    auto estoque = relatorioEstoque();
    std::stringstream csv;
    
    // Cabeçalho
    csv << "Código,Nome,Categoria,Quantidade,Estoque Mínimo,Valor Unitário,Valor Total,Status\n";
    
    // Dados
    for (const auto& item : estoque) {
        csv << item.codigo << ","
            << item.nome << ","
            << item.categoria << ","
            << item.quantidade_atual << ","
            << item.estoque_minimo << ","
            << std::fixed << std::setprecision(2) << item.valor_unitario << ","
            << std::fixed << std::setprecision(2) << item.valor_total << ","
            << item.status << "\n";
    }
    
    return csv.str();
}

bool RelatorioService::exportarRelatorioVendas(const std::string& arquivo, const QDate& inicio, const QDate& fim) {
    std::string conteudo = gerarRelatorioVendasCSV(inicio, fim);
    return salvarRelatorioArquivo(conteudo, arquivo);
}

bool RelatorioService::exportarRelatorioEstoque(const std::string& arquivo) {
    std::string conteudo = gerarRelatorioEstoqueCSV();
    return salvarRelatorioArquivo(conteudo, arquivo);
}

bool RelatorioService::salvarRelatorioArquivo(const std::string& conteudo, const std::string& arquivo) {
    std::ofstream file(arquivo);
    if (file.is_open()) {
        file << conteudo;
        file.close();
        return true;
    }
    return false;
}

std::string RelatorioService::formatarMoeda(double valor) {
    QLocale locale(QLocale::Portuguese, QLocale::Brazil);
    return locale.toCurrencyString(valor).toStdString();
}

std::string RelatorioService::formatarData(const QDate& data) {
    return data.toString("dd/MM/yyyy").toStdString();
}

std::string RelatorioService::formatarPorcentagem(double valor) {
    std::stringstream ss;
    ss << std::fixed << std::setprecision(2) << valor << "%";
    return ss.str();
}
