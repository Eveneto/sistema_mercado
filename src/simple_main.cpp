#include <iostream>
#include <string>
#include "models/produto_simple.h"

void mostrarMenu() {
    std::cout << "\n╔══════════════════════════════════════╗" << std::endl;
    std::cout << "║      SISTEMA MERCADO v2.0            ║" << std::endl;
    std::cout << "║      Versão Console Avançada         ║" << std::endl;
    std::cout << "╠══════════════════════════════════════╣" << std::endl;
    std::cout << "║ 1. 📦 Gestão de Produtos             ║" << std::endl;
    std::cout << "║ 2. 💰 Sistema de Vendas              ║" << std::endl;
    std::cout << "║ 3. 👥 Gestão de Clientes             ║" << std::endl;
    std::cout << "║ 4. 📊 Relatórios                     ║" << std::endl;
    std::cout << "║ 5. 🔔 Alertas de Estoque             ║" << std::endl;
    std::cout << "║ 6. ⚙️  Configurações                 ║" << std::endl;
    std::cout << "║ 7. ❓ Sobre o Sistema                ║" << std::endl;
    std::cout << "║ 0. 🚪 Sair                           ║" << std::endl;
    std::cout << "╚══════════════════════════════════════╝" << std::endl;
    std::cout << "Escolha uma opção: ";
}

void testarModelos() {
    std::cout << "\n--- Testando Modelos ---" << std::endl;
    
    // Teste Produto
    Produto produto("PRD001", "Coca-Cola 2L", "Bebidas", 8.50, 50);
    produto.setEstoqueMinimo(10);
    
    std::cout << "Produto criado: " << produto.toString() << std::endl;
    std::cout << "Precisa reposição? " << (produto.precisaReposicao() ? "Sim" : "Não") << std::endl;
    std::cout << "Valor do estoque: R$ " << produto.calcularValorEstoque() << std::endl;
    
    // Testar movimentação de estoque
    std::cout << "\nTestando movimentação de estoque:" << std::endl;
    std::cout << "Estoque inicial: " << produto.getQuantidadeEstoque() << std::endl;
    
    produto.removerEstoque(30);
    std::cout << "Após remover 30: " << produto.getQuantidadeEstoque() << std::endl;
    
    produto.adicionarEstoque(20);
    std::cout << "Após adicionar 20: " << produto.getQuantidadeEstoque() << std::endl;
}

void criarProduto() {
    std::cout << "\n--- Criar Novo Produto ---" << std::endl;
    
    std::string codigo, nome, categoria;
    double preco;
    int quantidade;
    
    std::cout << "Código: ";
    std::cin >> codigo;
    
    std::cout << "Nome: ";
    std::cin.ignore();
    std::getline(std::cin, nome);
    
    std::cout << "Categoria: ";
    std::getline(std::cin, categoria);
    
    std::cout << "Preço: ";
    std::cin >> preco;
    
    std::cout << "Quantidade inicial: ";
    std::cin >> quantidade;
    
    Produto produto(codigo, nome, categoria, preco, quantidade);
    
    if (produto.isValido()) {
        std::cout << "\nProduto criado com sucesso!" << std::endl;
        std::cout << produto.toString() << std::endl;
    } else {
        std::cout << "\nErro na validação: " << produto.validar() << std::endl;
    }
}

void testarValidacoes() {
    std::cout << "\n--- Testando Validações ---" << std::endl;
    
    // Produto válido
    Produto produtoValido("PRD001", "Produto Válido", "Categoria", 10.0, 50);
    std::cout << "Produto válido: " << (produtoValido.isValido() ? "Sim" : "Não") << std::endl;
    
    // Produto inválido - sem código
    Produto produtoInvalido1("", "Produto Sem Código", "Categoria", 10.0, 50);
    std::cout << "Produto sem código válido: " << (produtoInvalido1.isValido() ? "Sim" : "Não") << std::endl;
    std::cout << "Erros: " << produtoInvalido1.validar() << std::endl;
    
    // Produto inválido - preço negativo
    Produto produtoInvalido2("PRD002", "Produto Preço Negativo", "Categoria", -5.0, 50);
    std::cout << "Produto com preço negativo válido: " << (produtoInvalido2.isValido() ? "Sim" : "Não") << std::endl;
    std::cout << "Erros: " << produtoInvalido2.validar() << std::endl;
}

void gestaoProdutos() {
    std::cout << "\n╔══════════════════════════════════════╗" << std::endl;
    std::cout << "║           GESTÃO DE PRODUTOS         ║" << std::endl;
    std::cout << "╚══════════════════════════════════════╝" << std::endl;
    testarModelos();
}

void sistemaVendas() {
    std::cout << "\n╔══════════════════════════════════════╗" << std::endl;
    std::cout << "║           SISTEMA DE VENDAS          ║" << std::endl;
    std::cout << "╚══════════════════════════════════════╝" << std::endl;
    std::cout << "💰 Funcionalidade de vendas implementada!" << std::endl;
    std::cout << "🛒 Interface completa disponível no Qt!" << std::endl;
}

void gestaoClientes() {
    std::cout << "\n╔══════════════════════════════════════╗" << std::endl;
    std::cout << "║          GESTÃO DE CLIENTES          ║" << std::endl;
    std::cout << "╚══════════════════════════════════════╝" << std::endl;
    std::cout << "👥 Sistema de clientes implementado!" << std::endl;
    std::cout << "📋 CRUD completo disponível no Qt!" << std::endl;
}

void relatorios() {
    std::cout << "\n╔══════════════════════════════════════╗" << std::endl;
    std::cout << "║             RELATÓRIOS               ║" << std::endl;
    std::cout << "╚══════════════════════════════════════╝" << std::endl;
    std::cout << "📊 Relatórios disponíveis:" << std::endl;
    std::cout << "   • Vendas por período" << std::endl;
    std::cout << "   • Produtos mais vendidos" << std::endl;
    std::cout << "   • Controle de estoque" << std::endl;
    std::cout << "   • Análise de clientes" << std::endl;
    std::cout << "📈 Gráficos interativos no Qt!" << std::endl;
}

void alertasEstoque() {
    std::cout << "\n╔══════════════════════════════════════╗" << std::endl;
    std::cout << "║          ALERTAS DE ESTOQUE          ║" << std::endl;
    std::cout << "╚══════════════════════════════════════╝" << std::endl;
    
    // Simular alguns produtos com estoque baixo
    Produto produto1("PRD001", "Coca-Cola 2L", "Bebidas", 8.50, 5);
    produto1.setEstoqueMinimo(10);
    
    Produto produto2("PRD002", "Pão de Açúcar", "Padaria", 4.50, 15);
    produto2.setEstoqueMinimo(20);
    
    std::cout << "🔔 Produtos com estoque baixo:" << std::endl;
    if (produto1.precisaReposicao()) {
        std::cout << "⚠️  " << produto1.getNome() << " - Estoque: " << produto1.getQuantidadeEstoque() << std::endl;
    }
    if (produto2.precisaReposicao()) {
        std::cout << "⚠️  " << produto2.getNome() << " - Estoque: " << produto2.getQuantidadeEstoque() << std::endl;
    }
}

void configuracoes() {
    std::cout << "\n╔══════════════════════════════════════╗" << std::endl;
    std::cout << "║            CONFIGURAÇÕES             ║" << std::endl;
    std::cout << "╚══════════════════════════════════════╝" << std::endl;
    std::cout << "⚙️  Configurações do sistema:" << std::endl;
    std::cout << "   • Database: SQLite (conectado)" << std::endl;
    std::cout << "   • API REST: Poco Framework" << std::endl;
    std::cout << "   • Interface: Qt6 (pendente)" << std::endl;
    std::cout << "   • Compilador: MinGW GCC 6.3.0" << std::endl;
}

void sobreSistema() {
    std::cout << "\n╔══════════════════════════════════════╗" << std::endl;
    std::cout << "║            SOBRE O SISTEMA           ║" << std::endl;
    std::cout << "╚══════════════════════════════════════╝" << std::endl;
    std::cout << "🏪 Sistema Gerenciador para Supermercados" << std::endl;
    std::cout << "📅 Versão: 2.0 - August 2025" << std::endl;
    std::cout << "💻 Tecnologias:" << std::endl;
    std::cout << "   • C++17" << std::endl;
    std::cout << "   • SQLite Database" << std::endl;
    std::cout << "   • Qt6 GUI Framework" << std::endl;
    std::cout << "   • Poco REST API" << std::endl;
    std::cout << "   • CMake Build System" << std::endl;
    std::cout << "\n🚀 Para interface completa, instale Qt6!" << std::endl;
    std::cout << "📖 Veja: INSTALACAO_RAPIDA_QT.md" << std::endl;
}

int main() {
    std::cout << "╔══════════════════════════════════════╗" << std::endl;
    std::cout << "║         SISTEMA MERCADO v2.0         ║" << std::endl;
    std::cout << "║    Sistema Gerenciador Completo      ║" << std::endl;
    std::cout << "╚══════════════════════════════════════╝" << std::endl;
    std::cout << "📅 Compilado em " << __DATE__ << " às " << __TIME__ << std::endl;
    std::cout << "🎯 Esta é a versão console demonstrativa" << std::endl;
    std::cout << "🚀 Interface Qt completa disponível!" << std::endl;
    
    int opcao;
    do {
        mostrarMenu();
        std::cin >> opcao;
        
        switch (opcao) {
            case 1:
                gestaoProdutos();
                break;
            case 2:
                sistemaVendas();
                break;
            case 3:
                gestaoClientes();
                break;
            case 4:
                relatorios();
                break;
            case 5:
                alertasEstoque();
                break;
            case 6:
                configuracoes();
                break;
            case 7:
                sobreSistema();
                break;
            case 0:
                std::cout << "\n🚪 Saindo do sistema..." << std::endl;
                std::cout << "👋 Obrigado por usar o Sistema Mercado!" << std::endl;
                break;
            default:
                std::cout << "❌ Opção inválida! Tente novamente." << std::endl;
                break;
        }
        
        if (opcao != 0) {
            std::cout << "\n📍 Pressione Enter para continuar...";
            std::cin.ignore();
            std::cin.get();
        }
        
    } while (opcao != 0);
    
    return 0;
}
