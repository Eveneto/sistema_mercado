#ifndef DATABASE_MANAGER_H
#define DATABASE_MANAGER_H

#include <sqlite3.h>
#include <string>
#include <vector>
#include <memory>
#include "models/produto.h"
#include "models/venda.h"
#include "models/cliente.h"

class DatabaseManager {
private:
    sqlite3* db;
    std::string db_path;
    bool connected;

    // Métodos privados para criação de tabelas
    bool criarTabelaProdutos();
    bool criarTabelaVendas();
    bool criarTabelaItensVenda();
    bool criarTabelaClientes();
    bool criarIndices();

    // Métodos utilitários
    bool executarSQL(const std::string& sql);
    int getLastInsertId();

public:
    // Construtor e Destrutor
    explicit DatabaseManager(const std::string& db_path = "database/mercado.db");
    ~DatabaseManager();

    // Conexão
    bool conectar();
    void desconectar();
    bool isConectado() const { return connected; }

    // Inicialização
    bool inicializarBanco();
    bool criarTabelas();

    // Produtos - CRUD
    bool inserirProduto(Produto& produto);
    std::unique_ptr<Produto> buscarProduto(int id);
    std::unique_ptr<Produto> buscarProdutoPorCodigo(const std::string& codigo);
    std::vector<Produto> listarProdutos(bool apenas_ativos = true);
    std::vector<Produto> buscarProdutosPorNome(const std::string& nome);
    std::vector<Produto> buscarProdutosPorCategoria(const std::string& categoria);
    bool atualizarProduto(const Produto& produto);
    bool removerProduto(int id);
    bool inativarProduto(int id);

    // Estoque
    bool atualizarEstoque(int produto_id, int nova_quantidade);
    bool adicionarEstoque(int produto_id, int quantidade);
    bool removerEstoque(int produto_id, int quantidade);
    std::vector<Produto> listarProdutosEstoqueBaixo();
    std::vector<Produto> listarProdutosVencendo(int dias = 7);

    // Vendas - CRUD
    bool inserirVenda(Venda& venda);
    std::unique_ptr<Venda> buscarVenda(int id);
    std::vector<Venda> listarVendas(const QDateTime& data_inicio = QDateTime(),
                                   const QDateTime& data_fim = QDateTime());
    std::vector<Venda> listarVendasPorPeriodo(const QDate& inicio, const QDate& fim);
    bool cancelarVenda(int id);

    // Clientes - CRUD
    bool inserirCliente(Cliente& cliente);
    std::unique_ptr<Cliente> buscarCliente(int id);
    std::unique_ptr<Cliente> buscarClientePorCPF(const std::string& cpf);
    std::vector<Cliente> listarClientes(bool apenas_ativos = true);
    std::vector<Cliente> buscarClientesPorNome(const std::string& nome);
    bool atualizarCliente(const Cliente& cliente);
    bool removerCliente(int id);
    bool inativarCliente(int id);

    // Relatórios
    struct RelatorioVendas {
        QDate data;
        int quantidade_vendas;
        double total_vendas;
        int quantidade_itens;
    };

    struct ProdutoMaisVendido {
        int produto_id;
        std::string nome;
        int quantidade_vendida;
        double total_vendido;
    };

    std::vector<RelatorioVendas> relatorioVendasDiarias(const QDate& inicio, const QDate& fim);
    std::vector<RelatorioVendas> relatorioVendasMensais(int ano);
    std::vector<ProdutoMaisVendido> produtosMaisVendidos(const QDate& inicio, const QDate& fim, int limite = 10);
    double totalVendasPeriodo(const QDate& inicio, const QDate& fim);
    int quantidadeVendasPeriodo(const QDate& inicio, const QDate& fim);

    // Backup e Restore
    bool fazerBackup(const std::string& caminho_backup);
    bool restaurarBackup(const std::string& caminho_backup);

    // Utilitários
    std::string getUltimoErro() const;
    bool verificarIntegridadeBanco();
    bool limparDadosAntigos(int dias_manter = 365);
};

#endif // DATABASE_MANAGER_H
