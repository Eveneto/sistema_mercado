#ifndef RELATORIO_SERVICE_H
#define RELATORIO_SERVICE_H

#include <vector>
#include <string>
#include <memory>
#include <QDate>
#include <QDateTime>

class DatabaseManager;
class Produto;
class Venda;

class RelatorioService {
private:
    DatabaseManager* db_manager;

public:
    explicit RelatorioService(DatabaseManager* db_manager);
    ~RelatorioService() = default;

    // Estruturas para relatórios
    struct RelatorioVendas {
        QDate data;
        int quantidade_vendas;
        double total_vendas;
        int quantidade_itens;
        double ticket_medio;
    };

    struct ProdutoMaisVendido {
        int produto_id;
        std::string codigo;
        std::string nome;
        std::string categoria;
        int quantidade_vendida;
        double total_vendido;
        double percentual_vendas;
    };

    struct RelatorioEstoque {
        int produto_id;
        std::string codigo;
        std::string nome;
        std::string categoria;
        int quantidade_atual;
        int estoque_minimo;
        double valor_unitario;
        double valor_total;
        std::string status;
    };

    struct RelatorioFinanceiro {
        QDate periodo_inicio;
        QDate periodo_fim;
        double total_vendas;
        double total_custos;
        double lucro_bruto;
        double margem_lucro;
        int quantidade_vendas;
        double ticket_medio;
    };

    // Relatórios de Vendas
    std::vector<RelatorioVendas> relatorioVendasDiarias(const QDate& inicio, const QDate& fim);
    std::vector<RelatorioVendas> relatorioVendasMensais(int ano);
    RelatorioFinanceiro relatorioFinanceiroPeriodo(const QDate& inicio, const QDate& fim);
    
    // Relatórios de Produtos
    std::vector<ProdutoMaisVendido> produtosMaisVendidos(const QDate& inicio, const QDate& fim, int limite = 10);
    std::vector<ProdutoMaisVendido> produtosMenosVendidos(const QDate& inicio, const QDate& fim, int limite = 10);
    std::vector<RelatorioEstoque> relatorioEstoque();
    std::vector<RelatorioEstoque> relatorioEstoqueBaixo();
    std::vector<RelatorioEstoque> relatorioEstoquePorCategoria(const std::string& categoria);
    
    // Análises Específicas
    struct AnaliseVendas {
        double crescimento_vendas;
        double crescimento_quantidade;
        QDate dia_maior_venda;
        QDate dia_menor_venda;
        std::string forma_pagamento_preferida;
        double percentual_cancelamentos;
    };
    
    AnaliseVendas analisarVendas(const QDate& inicio, const QDate& fim);
    
    // Relatórios em formato texto/CSV
    std::string gerarRelatorioVendasCSV(const QDate& inicio, const QDate& fim);
    std::string gerarRelatorioProdutosCSV();
    std::string gerarRelatorioEstoqueCSV();
    
    // Exportação
    bool exportarRelatorioVendas(const std::string& arquivo, const QDate& inicio, const QDate& fim);
    bool exportarRelatorioEstoque(const std::string& arquivo);
    bool exportarRelatorioProdutos(const std::string& arquivo);
    
    // Dashboards e KPIs
    struct KPIs {
        double vendas_hoje;
        double vendas_ontem;
        double vendas_mes_atual;
        double vendas_mes_anterior;
        int produtos_estoque_baixo;
        int produtos_sem_estoque;
        int total_produtos_ativos;
        double valor_total_estoque;
        double ticket_medio_mes;
    };
    
    KPIs obterKPIs();
    
    // Previsões e Tendências
    struct TendenciaVendas {
        double media_diaria;
        double tendencia_crescimento;
        double previsao_proximo_mes;
        std::vector<double> vendas_ultimos_30_dias;
    };
    
    TendenciaVendas calcularTendenciaVendas();
    
    // Relatórios de Alerta
    struct AlertaRelatorio {
        std::string tipo;
        std::string descricao;
        std::string nivel; // "baixo", "medio", "alto"
        QDateTime data_alerta;
    };
    
    std::vector<AlertaRelatorio> gerarAlertas();
    
    // Utilitários
    bool salvarRelatorioArquivo(const std::string& conteudo, const std::string& arquivo);
    std::string formatarMoeda(double valor);
    std::string formatarData(const QDate& data);
    std::string formatarPorcentagem(double valor);
};

#endif // RELATORIO_SERVICE_H
