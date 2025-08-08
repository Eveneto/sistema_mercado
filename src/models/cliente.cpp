#include "models/cliente.h"
#include <sstream>
#include <regex>
#include <algorithm>

Cliente::Cliente() 
    : id(0), data_cadastro(QDateTime::currentDateTime()), ativo(true),
      limite_credito(0.0), saldo_devedor(0.0) {
}

Cliente::Cliente(const std::string& nome, const std::string& cpf)
    : id(0), nome(nome), cpf(cpf), data_cadastro(QDateTime::currentDateTime()),
      ativo(true), limite_credito(0.0), saldo_devedor(0.0) {
}

Cliente::Cliente(const std::string& nome, const std::string& cpf, 
                const std::string& telefone, const std::string& email)
    : id(0), nome(nome), cpf(cpf), telefone(telefone), email(email),
      data_cadastro(QDateTime::currentDateTime()), ativo(true),
      limite_credito(0.0), saldo_devedor(0.0) {
}

bool Cliente::podeComprarCredito(double valor) const {
    return (saldo_devedor + valor) <= limite_credito;
}

void Cliente::adicionarDebito(double valor) {
    if (valor > 0) {
        saldo_devedor += valor;
    }
}

bool Cliente::pagarDebito(double valor) {
    if (valor > 0 && valor <= saldo_devedor) {
        saldo_devedor -= valor;
        return true;
    }
    return false;
}

double Cliente::getCreditoDisponivel() const {
    return limite_credito - saldo_devedor;
}

bool Cliente::isValido() const {
    return !nome.empty() && validarCPF(cpf);
}

std::string Cliente::validar() const {
    std::stringstream erros;
    
    if (nome.empty()) {
        erros << "Nome não pode estar vazio. ";
    }
    
    if (!validarCPF(cpf)) {
        erros << "CPF inválido. ";
    }
    
    if (!email.empty() && !validarEmail(email)) {
        erros << "Email inválido. ";
    }
    
    if (limite_credito < 0) {
        erros << "Limite de crédito não pode ser negativo. ";
    }
    
    if (saldo_devedor < 0) {
        erros << "Saldo devedor não pode ser negativo. ";
    }
    
    return erros.str();
}

bool Cliente::validarCPF(const std::string& cpf) {
    if (cpf.empty()) {
        return false; // CPF pode ser opcional
    }
    
    // Remove caracteres não numéricos
    std::string cpf_numeros;
    std::copy_if(cpf.begin(), cpf.end(), std::back_inserter(cpf_numeros),
                [](char c) { return std::isdigit(c); });
    
    // Verifica se tem 11 dígitos
    if (cpf_numeros.length() != 11) {
        return false;
    }
    
    // Verifica se todos os dígitos são iguais
    if (std::all_of(cpf_numeros.begin(), cpf_numeros.end(),
                   [&](char c) { return c == cpf_numeros[0]; })) {
        return false;
    }
    
    // Validação dos dígitos verificadores
    // Primeiro dígito
    int soma = 0;
    for (int i = 0; i < 9; i++) {
        soma += (cpf_numeros[i] - '0') * (10 - i);
    }
    int resto = soma % 11;
    int digito1 = (resto < 2) ? 0 : 11 - resto;
    
    if (digito1 != (cpf_numeros[9] - '0')) {
        return false;
    }
    
    // Segundo dígito
    soma = 0;
    for (int i = 0; i < 10; i++) {
        soma += (cpf_numeros[i] - '0') * (11 - i);
    }
    resto = soma % 11;
    int digito2 = (resto < 2) ? 0 : 11 - resto;
    
    return digito2 == (cpf_numeros[10] - '0');
}

bool Cliente::validarEmail(const std::string& email) {
    if (email.empty()) {
        return true; // Email é opcional
    }
    
    const std::regex pattern(
        R"(^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$)"
    );
    
    return std::regex_match(email, pattern);
}

std::string Cliente::toString() const {
    std::stringstream ss;
    ss << "Cliente{id=" << id 
       << ", nome='" << nome 
       << "', cpf='" << cpf 
       << "', telefone='" << telefone
       << "', email='" << email 
       << "', ativo=" << (ativo ? "true" : "false")
       << ", limite_credito=" << limite_credito
       << ", saldo_devedor=" << saldo_devedor << "}";
    return ss.str();
}
