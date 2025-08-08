#ifndef VENDA_H
#define VENDA_H

#include <string>
#include <vector>
#include <QDateTime>

struct ItemVenda {
    int produto_id;
    std::string produto_codigo;
    std::string produto_nome;
    int quantidade;
    double preco_unitario;
    double subtotal;
    
    ItemVenda(int prod_id, const std::string& codigo, const std::string& nome,
              int qtd, double preco) 
        : produto_id(prod_id), produto_codigo(codigo), produto_nome(nome),
          quantidade(qtd), preco_unitario(preco), subtotal(qtd * preco) {}
};

enum class FormaPagamento {
    DINHEIRO,
    CARTAO_CREDITO,
    CARTAO_DEBITO,
    PIX,
    OUTROS
};

class Venda {
private:
    int id;
    QDateTime data_venda;
    std::vector<ItemVenda> itens;
    double total;
    FormaPagamento forma_pagamento;
    double valor_pago;
    double troco;
    std::string observacoes;
    int cliente_id;
    bool cancelada;

public:
    // Construtores
    Venda();
    explicit Venda(FormaPagamento forma_pagamento);
    
    // Destrutor
    ~Venda() = default;

    // Getters
    int getId() const { return id; }
    QDateTime getDataVenda() const { return data_venda; }
    const std::vector<ItemVenda>& getItens() const { return itens; }
    double getTotal() const { return total; }
    FormaPagamento getFormaPagamento() const { return forma_pagamento; }
    double getValorPago() const { return valor_pago; }
    double getTroco() const { return troco; }
    std::string getObservacoes() const { return observacoes; }
    int getClienteId() const { return cliente_id; }
    bool isCancelada() const { return cancelada; }

    // Setters
    void setId(int id) { this->id = id; }
    void setDataVenda(const QDateTime& data) { this->data_venda = data; }
    void setFormaPagamento(FormaPagamento forma) { this->forma_pagamento = forma; }
    void setValorPago(double valor) { this->valor_pago = valor; }
    void setObservacoes(const std::string& obs) { this->observacoes = obs; }
    void setClienteId(int cliente_id) { this->cliente_id = cliente_id; }
    void setCancelada(bool cancelada) { this->cancelada = cancelada; }

    // Métodos de negócio
    void adicionarItem(const ItemVenda& item);
    void adicionarItem(int produto_id, const std::string& codigo, 
                      const std::string& nome, int quantidade, double preco);
    bool removerItem(int produto_id);
    void limparItens();
    void calcularTotal();
    double calcularTroco();
    bool finalizarVenda(double valor_pago);

    // Validações
    bool isValida() const;
    std::string validar() const;
    bool temItens() const { return !itens.empty(); }
    int getQuantidadeItens() const { return itens.size(); }

    // Utilitários
    std::string getFormaPagamentoString() const;
    static std::string formaPagamentoToString(FormaPagamento forma);
    static FormaPagamento stringToFormaPagamento(const std::string& forma);

    // Conversões
    std::string toString() const;
};

#endif // VENDA_H
