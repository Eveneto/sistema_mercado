#ifndef VENDA_SERVICE_H
#define VENDA_SERVICE_H

#include <vector>
#include <memory>
#include "models/venda.h"
#include <QDate>

class DatabaseManager;

class VendaService {
private:
    DatabaseManager* db_manager;

public:
    explicit VendaService(DatabaseManager* db_manager);
    ~VendaService() = default;

    // CRUD básico
    bool registrarVenda(Venda& venda);
    std::unique_ptr<Venda> buscarVenda(int id);
    std::vector<Venda> listarVendas();
    std::vector<Venda> listarVendasPorPeriodo(const QDate& inicio, const QDate& fim);
    bool cancelarVenda(int id);

    // Processamento de vendas
    bool processarVenda(Venda& venda);
    bool validarEstoque(const Venda& venda);
    bool atualizarEstoqueVenda(const Venda& venda);
    bool estornarEstoque(int venda_id);

    // Consultas específicas
    std::vector<Venda> buscarVendasPorCliente(int cliente_id);
    std::vector<Venda> buscarVendasPorFormaPagamento(FormaPagamento forma);
    std::vector<Venda> buscarVendasCanceladas();
    
    // Estatísticas e métricas
    double calcularTotalVendasDia(const QDate& data);
    double calcularTotalVendasMes(int mes, int ano);
    double calcularTotalVendasAno(int ano);
    int contarVendasDia(const QDate& data);
    int contarVendasMes(int mes, int ano);
    
    // Validações
    bool validarVenda(const Venda& venda);
    std::string obterErrosValidacao(const Venda& venda);
    
    // Relatórios de vendas
    struct ResumoVendas {
        QDate data;
        int quantidade_vendas;
        double total_vendas;
        double ticket_medio;
    };
    
    struct VendaPorFormaPagamento {
        FormaPagamento forma;
        int quantidade;
        double total;
        double percentual;
    };
    
    std::vector<ResumoVendas> resumoVendasPorDia(const QDate& inicio, const QDate& fim);
    std::vector<VendaPorFormaPagamento> vendasPorFormaPagamento(const QDate& inicio, const QDate& fim);
    
    // Análises
    double calcularTicketMedio(const QDate& inicio, const QDate& fim);
    FormaPagamento formaPagamentoMaisUsada(const QDate& inicio, const QDate& fim);
    QDate diaMaiorVenda(const QDate& inicio, const QDate& fim);
    
    // Utilitários
    bool vendaExiste(int id);
    bool vendaPodeSerCancelada(int id);
};

#endif // VENDA_SERVICE_H
