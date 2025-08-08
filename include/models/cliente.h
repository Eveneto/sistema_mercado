#ifndef CLIENTE_H
#define CLIENTE_H

#include <string>
#include <QDateTime>

class Cliente {
private:
    int id;
    std::string nome;
    std::string cpf;
    std::string telefone;
    std::string email;
    std::string endereco;
    QDateTime data_cadastro;
    bool ativo;
    double limite_credito;
    double saldo_devedor;

public:
    // Construtores
    Cliente();
    Cliente(const std::string& nome, const std::string& cpf);
    Cliente(const std::string& nome, const std::string& cpf, 
            const std::string& telefone, const std::string& email);
    
    // Destrutor
    ~Cliente() = default;

    // Getters
    int getId() const { return id; }
    std::string getNome() const { return nome; }
    std::string getCpf() const { return cpf; }
    std::string getTelefone() const { return telefone; }
    std::string getEmail() const { return email; }
    std::string getEndereco() const { return endereco; }
    QDateTime getDataCadastro() const { return data_cadastro; }
    bool isAtivo() const { return ativo; }
    double getLimiteCredito() const { return limite_credito; }
    double getSaldoDevedor() const { return saldo_devedor; }

    // Setters
    void setId(int id) { this->id = id; }
    void setNome(const std::string& nome) { this->nome = nome; }
    void setCpf(const std::string& cpf) { this->cpf = cpf; }
    void setTelefone(const std::string& telefone) { this->telefone = telefone; }
    void setEmail(const std::string& email) { this->email = email; }
    void setEndereco(const std::string& endereco) { this->endereco = endereco; }
    void setAtivo(bool ativo) { this->ativo = ativo; }
    void setLimiteCredito(double limite) { this->limite_credito = limite; }
    void setSaldoDevedor(double saldo) { this->saldo_devedor = saldo; }

    // Métodos de negócio
    bool podeComprarCredito(double valor) const;
    void adicionarDebito(double valor);
    bool pagarDebito(double valor);
    double getCreditoDisponivel() const;

    // Validações
    bool isValido() const;
    std::string validar() const;
    static bool validarCPF(const std::string& cpf);
    static bool validarEmail(const std::string& email);

    // Conversões
    std::string toString() const;
};

#endif // CLIENTE_H
