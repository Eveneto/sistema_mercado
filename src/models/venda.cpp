#include "models/venda.h"
#include <algorithm>
#include <sstream>
#include <cmath>

Venda::Venda() 
    : id(0), data_venda(QDateTime::currentDateTime()), total(0.0), 
      forma_pagamento(FormaPagamento::DINHEIRO), valor_pago(0.0), 
      troco(0.0), cliente_id(0), cancelada(false) {
}

Venda::Venda(FormaPagamento forma_pagamento)
    : id(0), data_venda(QDateTime::currentDateTime()), total(0.0),
      forma_pagamento(forma_pagamento), valor_pago(0.0), 
      troco(0.0), cliente_id(0), cancelada(false) {
}

void Venda::adicionarItem(const ItemVenda& item) {
    // Verificar se o produto já existe na venda
    auto it = std::find_if(itens.begin(), itens.end(),
        [&item](const ItemVenda& i) { return i.produto_id == item.produto_id; });
    
    if (it != itens.end()) {
        // Produto já existe, atualizar quantidade
        it->quantidade += item.quantidade;
        it->subtotal = it->quantidade * it->preco_unitario;
    } else {
        // Novo produto
        itens.push_back(item);
    }
    
    calcularTotal();
}

void Venda::adicionarItem(int produto_id, const std::string& codigo, 
                         const std::string& nome, int quantidade, double preco) {
    ItemVenda item(produto_id, codigo, nome, quantidade, preco);
    adicionarItem(item);
}

bool Venda::removerItem(int produto_id) {
    auto it = std::find_if(itens.begin(), itens.end(),
        [produto_id](const ItemVenda& item) { return item.produto_id == produto_id; });
    
    if (it != itens.end()) {
        itens.erase(it);
        calcularTotal();
        return true;
    }
    return false;
}

void Venda::limparItens() {
    itens.clear();
    total = 0.0;
}

void Venda::calcularTotal() {
    total = 0.0;
    for (const auto& item : itens) {
        total += item.subtotal;
    }
}

double Venda::calcularTroco() {
    if (valor_pago >= total) {
        troco = valor_pago - total;
        return troco;
    }
    return 0.0;
}

bool Venda::finalizarVenda(double valor_pago) {
    this->valor_pago = valor_pago;
    
    if (valor_pago >= total) {
        calcularTroco();
        return true;
    }
    return false;
}

bool Venda::isValida() const {
    return !itens.empty() && total > 0 && !cancelada;
}

std::string Venda::validar() const {
    std::stringstream erros;
    
    if (itens.empty()) {
        erros << "Venda deve ter pelo menos um item. ";
    }
    
    if (total <= 0) {
        erros << "Total da venda deve ser maior que zero. ";
    }
    
    if (cancelada) {
        erros << "Venda está cancelada. ";
    }
    
    return erros.str();
}

std::string Venda::getFormaPagamentoString() const {
    return formaPagamentoToString(forma_pagamento);
}

std::string Venda::formaPagamentoToString(FormaPagamento forma) {
    switch (forma) {
        case FormaPagamento::DINHEIRO: return "Dinheiro";
        case FormaPagamento::CARTAO_CREDITO: return "Cartão de Crédito";
        case FormaPagamento::CARTAO_DEBITO: return "Cartão de Débito";
        case FormaPagamento::PIX: return "PIX";
        case FormaPagamento::OUTROS: return "Outros";
        default: return "Desconhecido";
    }
}

FormaPagamento Venda::stringToFormaPagamento(const std::string& forma) {
    if (forma == "Dinheiro") return FormaPagamento::DINHEIRO;
    if (forma == "Cartão de Crédito") return FormaPagamento::CARTAO_CREDITO;
    if (forma == "Cartão de Débito") return FormaPagamento::CARTAO_DEBITO;
    if (forma == "PIX") return FormaPagamento::PIX;
    if (forma == "Outros") return FormaPagamento::OUTROS;
    return FormaPagamento::DINHEIRO;
}

std::string Venda::toString() const {
    std::stringstream ss;
    ss << "Venda{id=" << id 
       << ", data='" << data_venda.toString().toStdString()
       << "', total=" << total 
       << ", forma_pagamento='" << getFormaPagamentoString()
       << "', itens=" << itens.size()
       << ", cancelada=" << (cancelada ? "true" : "false") << "}";
    return ss.str();
}
