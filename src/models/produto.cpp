#include "models/produto.h"
#include <sstream>
#include <QDate>

Produto::Produto() 
    : id(0), preco(0.0), quantidade_estoque(0), estoque_minimo(10), 
      data_criacao(QDateTime::currentDateTime()), ativo(true) {
}

Produto::Produto(const std::string& codigo, const std::string& nome, 
                 const std::string& categoria, double preco, int quantidade)
    : id(0), codigo(codigo), nome(nome), categoria(categoria), 
      preco(preco), quantidade_estoque(quantidade), estoque_minimo(10),
      data_criacao(QDateTime::currentDateTime()), ativo(true) {
}

bool Produto::precisaReposicao() const {
    return quantidade_estoque <= estoque_minimo;
}

bool Produto::estaVencido() const {
    if (!data_vencimento.isValid()) {
        return false;
    }
    return QDateTime::currentDateTime() > data_vencimento;
}

bool Produto::estaProximoVencimento(int dias) const {
    if (!data_vencimento.isValid()) {
        return false;
    }
    QDateTime limite = QDateTime::currentDateTime().addDays(dias);
    return data_vencimento <= limite;
}

void Produto::adicionarEstoque(int quantidade) {
    if (quantidade > 0) {
        quantidade_estoque += quantidade;
    }
}

bool Produto::removerEstoque(int quantidade) {
    if (quantidade > 0 && quantidade <= quantidade_estoque) {
        quantidade_estoque -= quantidade;
        return true;
    }
    return false;
}

double Produto::calcularValorEstoque() const {
    return preco * quantidade_estoque;
}

bool Produto::isValido() const {
    return !codigo.empty() && !nome.empty() && preco >= 0 && quantidade_estoque >= 0;
}

std::string Produto::validar() const {
    std::stringstream erros;
    
    if (codigo.empty()) {
        erros << "Código não pode estar vazio. ";
    }
    
    if (nome.empty()) {
        erros << "Nome não pode estar vazio. ";
    }
    
    if (preco < 0) {
        erros << "Preço não pode ser negativo. ";
    }
    
    if (quantidade_estoque < 0) {
        erros << "Quantidade em estoque não pode ser negativa. ";
    }
    
    return erros.str();
}

std::string Produto::toString() const {
    std::stringstream ss;
    ss << "Produto{id=" << id 
       << ", codigo='" << codigo 
       << "', nome='" << nome 
       << "', categoria='" << categoria
       << "', preco=" << preco 
       << ", estoque=" << quantidade_estoque 
       << ", ativo=" << (ativo ? "true" : "false") << "}";
    return ss.str();
}
