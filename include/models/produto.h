#ifndef PRODUTO_H
#define PRODUTO_H

#include <string>
#include <QDateTime>

class Produto {
private:
    int id;
    std::string codigo;
    std::string nome;
    std::string categoria;
    double preco;
    int quantidade_estoque;
    int estoque_minimo;
    QDateTime data_criacao;
    QDateTime data_vencimento;
    bool ativo;

public:
    // Construtores
    Produto();
    Produto(const std::string& codigo, const std::string& nome, 
            const std::string& categoria, double preco, int quantidade);
    
    // Destrutor
    ~Produto() = default;

    // Getters
    int getId() const { return id; }
    std::string getCodigo() const { return codigo; }
    std::string getNome() const { return nome; }
    std::string getCategoria() const { return categoria; }
    double getPreco() const { return preco; }
    int getQuantidadeEstoque() const { return quantidade_estoque; }
    int getEstoqueMinimo() const { return estoque_minimo; }
    QDateTime getDataCriacao() const { return data_criacao; }
    QDateTime getDataVencimento() const { return data_vencimento; }
    bool isAtivo() const { return ativo; }

    // Setters
    void setId(int id) { this->id = id; }
    void setCodigo(const std::string& codigo) { this->codigo = codigo; }
    void setNome(const std::string& nome) { this->nome = nome; }
    void setCategoria(const std::string& categoria) { this->categoria = categoria; }
    void setPreco(double preco) { this->preco = preco; }
    void setQuantidadeEstoque(int quantidade) { this->quantidade_estoque = quantidade; }
    void setEstoqueMinimo(int minimo) { this->estoque_minimo = minimo; }
    void setDataVencimento(const QDateTime& data) { this->data_vencimento = data; }
    void setAtivo(bool ativo) { this->ativo = ativo; }

    // Métodos de negócio
    bool precisaReposicao() const;
    bool estaVencido() const;
    bool estaProximoVencimento(int dias = 7) const;
    void adicionarEstoque(int quantidade);
    bool removerEstoque(int quantidade);
    double calcularValorEstoque() const;

    // Validações
    bool isValido() const;
    std::string validar() const;

    // Conversões
    std::string toString() const;
};

#endif // PRODUTO_H
