#ifndef PRODUTO_SERVICE_H
#define PRODUTO_SERVICE_H

#include <vector>
#include <memory>
#include <string>
#include "models/produto.h"

class DatabaseManager;

class ProdutoService {
private:
    DatabaseManager* db_manager;

public:
    explicit ProdutoService(DatabaseManager* db_manager);
    ~ProdutoService() = default;

    // CRUD básico
    bool adicionarProduto(Produto& produto);
    std::unique_ptr<Produto> buscarProduto(int id);
    std::unique_ptr<Produto> buscarProdutoPorCodigo(const std::string& codigo);
    std::vector<Produto> listarTodosProdutos();
    std::vector<Produto> listarProdutosAtivos();
    bool atualizarProduto(const Produto& produto);
    bool removerProduto(int id);
    bool inativarProduto(int id);

    // Busca e filtros
    std::vector<Produto> buscarPorNome(const std::string& nome);
    std::vector<Produto> buscarPorCategoria(const std::string& categoria);
    std::vector<Produto> filtrarPorPreco(double preco_min, double preco_max);
    std::vector<std::string> listarCategorias();

    // Gestão de estoque
    bool adicionarEstoque(int produto_id, int quantidade);
    bool removerEstoque(int produto_id, int quantidade);
    bool atualizarEstoque(int produto_id, int nova_quantidade);
    std::vector<Produto> listarProdutosEstoqueBaixo();
    std::vector<Produto> listarProdutosVencendo(int dias = 7);
    std::vector<Produto> listarProdutosSemEstoque();

    // Validações e regras de negócio
    bool validarCodigo(const std::string& codigo, int produto_id = 0);
    std::string gerarProximoCodigo();
    bool produtoTemVendasAssociadas(int produto_id);
    
    // Relatórios e estatísticas
    int contarProdutosAtivos();
    int contarProdutosEstoqueBaixo();
    double calcularValorTotalEstoque();
    double calcularValorEstoquePorCategoria(const std::string& categoria);
    
    // Operações em lote
    bool importarProdutos(const std::vector<Produto>& produtos);
    std::vector<Produto> exportarProdutos();
    bool atualizarPrecosEmLote(const std::vector<std::pair<int, double>>& precos);
    
    // Alertas
    struct AlertaEstoque {
        Produto produto;
        enum TipoAlerta {
            ESTOQUE_BAIXO,
            SEM_ESTOQUE,
            PROXIMO_VENCIMENTO,
            VENCIDO
        } tipo;
        std::string mensagem;
    };
    
    std::vector<AlertaEstoque> verificarAlertas();
};

#endif // PRODUTO_SERVICE_H
