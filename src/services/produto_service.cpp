#include "services/produto_service.h"
#include "database/database_manager.h"
#include <algorithm>
#include <sstream>
#include <iomanip>

ProdutoService::ProdutoService(DatabaseManager* db_manager) 
    : db_manager(db_manager) {
}

bool ProdutoService::adicionarProduto(Produto& produto) {
    if (!produto.isValido()) {
        return false;
    }
    
    // Verificar se o código já existe
    if (!validarCodigo(produto.getCodigo())) {
        return false;
    }
    
    return db_manager->inserirProduto(produto);
}

std::unique_ptr<Produto> ProdutoService::buscarProduto(int id) {
    return db_manager->buscarProduto(id);
}

std::unique_ptr<Produto> ProdutoService::buscarProdutoPorCodigo(const std::string& codigo) {
    return db_manager->buscarProdutoPorCodigo(codigo);
}

std::vector<Produto> ProdutoService::listarTodosProdutos() {
    return db_manager->listarProdutos(false); // incluir inativos
}

std::vector<Produto> ProdutoService::listarProdutosAtivos() {
    return db_manager->listarProdutos(true); // apenas ativos
}

bool ProdutoService::atualizarProduto(const Produto& produto) {
    if (!produto.isValido()) {
        return false;
    }
    
    // Verificar se o código não conflita com outro produto
    auto produto_existente = buscarProdutoPorCodigo(produto.getCodigo());
    if (produto_existente && produto_existente->getId() != produto.getId()) {
        return false;
    }
    
    return db_manager->atualizarProduto(produto);
}

bool ProdutoService::removerProduto(int id) {
    // Verificar se o produto tem vendas associadas
    if (produtoTemVendasAssociadas(id)) {
        return false;
    }
    
    return db_manager->removerProduto(id);
}

bool ProdutoService::inativarProduto(int id) {
    return db_manager->inativarProduto(id);
}

std::vector<Produto> ProdutoService::buscarPorNome(const std::string& nome) {
    return db_manager->buscarProdutosPorNome(nome);
}

std::vector<Produto> ProdutoService::buscarPorCategoria(const std::string& categoria) {
    return db_manager->buscarProdutosPorCategoria(categoria);
}

std::vector<Produto> ProdutoService::filtrarPorPreco(double preco_min, double preco_max) {
    auto produtos = listarProdutosAtivos();
    std::vector<Produto> resultado;
    
    std::copy_if(produtos.begin(), produtos.end(), 
                std::back_inserter(resultado),
                [preco_min, preco_max](const Produto& p) {
                    return p.getPreco() >= preco_min && p.getPreco() <= preco_max;
                });
    
    return resultado;
}

std::vector<std::string> ProdutoService::listarCategorias() {
    auto produtos = listarTodosProdutos();
    std::vector<std::string> categorias;
    
    for (const auto& produto : produtos) {
        std::string categoria = produto.getCategoria();
        if (!categoria.empty() && 
            std::find(categorias.begin(), categorias.end(), categoria) == categorias.end()) {
            categorias.push_back(categoria);
        }
    }
    
    std::sort(categorias.begin(), categorias.end());
    return categorias;
}

bool ProdutoService::adicionarEstoque(int produto_id, int quantidade) {
    return db_manager->adicionarEstoque(produto_id, quantidade);
}

bool ProdutoService::removerEstoque(int produto_id, int quantidade) {
    return db_manager->removerEstoque(produto_id, quantidade);
}

bool ProdutoService::atualizarEstoque(int produto_id, int nova_quantidade) {
    return db_manager->atualizarEstoque(produto_id, nova_quantidade);
}

std::vector<Produto> ProdutoService::listarProdutosEstoqueBaixo() {
    return db_manager->listarProdutosEstoqueBaixo();
}

std::vector<Produto> ProdutoService::listarProdutosVencendo(int dias) {
    return db_manager->listarProdutosVencendo(dias);
}

std::vector<Produto> ProdutoService::listarProdutosSemEstoque() {
    auto produtos = listarProdutosAtivos();
    std::vector<Produto> sem_estoque;
    
    std::copy_if(produtos.begin(), produtos.end(),
                std::back_inserter(sem_estoque),
                [](const Produto& p) {
                    return p.getQuantidadeEstoque() == 0;
                });
    
    return sem_estoque;
}

bool ProdutoService::validarCodigo(const std::string& codigo, int produto_id) {
    if (codigo.empty()) {
        return false;
    }
    
    auto produto = buscarProdutoPorCodigo(codigo);
    return !produto || produto->getId() == produto_id;
}

std::string ProdutoService::gerarProximoCodigo() {
    auto produtos = listarTodosProdutos();
    int maior_numero = 0;
    
    for (const auto& produto : produtos) {
        std::string codigo = produto.getCodigo();
        if (codigo.length() >= 4 && codigo.substr(0, 3) == "PRD") {
            try {
                int numero = std::stoi(codigo.substr(3));
                maior_numero = std::max(maior_numero, numero);
            } catch (...) {
                // Ignorar códigos que não seguem o padrão
            }
        }
    }
    
    std::stringstream ss;
    ss << "PRD" << std::setfill('0') << std::setw(4) << (maior_numero + 1);
    return ss.str();
}

bool ProdutoService::produtoTemVendasAssociadas(int produto_id) {
    // Esta função seria implementada consultando a tabela de itens_venda
    // Por enquanto, retornamos false
    return false;
}

int ProdutoService::contarProdutosAtivos() {
    return static_cast<int>(listarProdutosAtivos().size());
}

int ProdutoService::contarProdutosEstoqueBaixo() {
    return static_cast<int>(listarProdutosEstoqueBaixo().size());
}

double ProdutoService::calcularValorTotalEstoque() {
    auto produtos = listarProdutosAtivos();
    double total = 0.0;
    
    for (const auto& produto : produtos) {
        total += produto.calcularValorEstoque();
    }
    
    return total;
}

double ProdutoService::calcularValorEstoquePorCategoria(const std::string& categoria) {
    auto produtos = buscarPorCategoria(categoria);
    double total = 0.0;
    
    for (const auto& produto : produtos) {
        if (produto.isAtivo()) {
            total += produto.calcularValorEstoque();
        }
    }
    
    return total;
}

bool ProdutoService::importarProdutos(const std::vector<Produto>& produtos) {
    bool sucesso = true;
    
    for (auto produto : produtos) {
        if (!adicionarProduto(produto)) {
            sucesso = false;
        }
    }
    
    return sucesso;
}

std::vector<Produto> ProdutoService::exportarProdutos() {
    return listarTodosProdutos();
}

bool ProdutoService::atualizarPrecosEmLote(const std::vector<std::pair<int, double>>& precos) {
    bool sucesso = true;
    
    for (const auto& par : precos) {
        auto produto = buscarProduto(par.first);
        if (produto) {
            produto->setPreco(par.second);
            if (!atualizarProduto(*produto)) {
                sucesso = false;
            }
        } else {
            sucesso = false;
        }
    }
    
    return sucesso;
}

std::vector<ProdutoService::AlertaEstoque> ProdutoService::verificarAlertas() {
    std::vector<AlertaEstoque> alertas;
    
    // Produtos sem estoque
    auto sem_estoque = listarProdutosSemEstoque();
    for (const auto& produto : sem_estoque) {
        AlertaEstoque alerta;
        alerta.produto = produto;
        alerta.tipo = AlertaEstoque::SEM_ESTOQUE;
        alerta.mensagem = "Produto sem estoque: " + produto.getNome();
        alertas.push_back(alerta);
    }
    
    // Produtos com estoque baixo
    auto estoque_baixo = listarProdutosEstoqueBaixo();
    for (const auto& produto : estoque_baixo) {
        if (produto.getQuantidadeEstoque() > 0) { // Para evitar duplicação com sem estoque
            AlertaEstoque alerta;
            alerta.produto = produto;
            alerta.tipo = AlertaEstoque::ESTOQUE_BAIXO;
            alerta.mensagem = "Estoque baixo: " + produto.getNome() + 
                             " (Atual: " + std::to_string(produto.getQuantidadeEstoque()) + 
                             ", Mínimo: " + std::to_string(produto.getEstoqueMinimo()) + ")";
            alertas.push_back(alerta);
        }
    }
    
    // Produtos próximos ao vencimento
    auto vencendo = listarProdutosVencendo(7);
    for (const auto& produto : vencendo) {
        if (!produto.estaVencido()) {
            AlertaEstoque alerta;
            alerta.produto = produto;
            alerta.tipo = AlertaEstoque::PROXIMO_VENCIMENTO;
            alerta.mensagem = "Produto próximo ao vencimento: " + produto.getNome();
            alertas.push_back(alerta);
        }
    }
    
    // Produtos vencidos
    auto produtos = listarProdutosAtivos();
    for (const auto& produto : produtos) {
        if (produto.estaVencido()) {
            AlertaEstoque alerta;
            alerta.produto = produto;
            alerta.tipo = AlertaEstoque::VENCIDO;
            alerta.mensagem = "Produto vencido: " + produto.getNome();
            alertas.push_back(alerta);
        }
    }
    
    return alertas;
}
