#include <iostream>
#include <string>
#include <vector>
#include <memory>

#ifdef QT_VERSION
#include <QApplication>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QTableWidget>
#include <QHeaderView>
#include <QMessageBox>
#include <QWidget>
#include <QLineEdit>
#include <QFormLayout>
#include <QDialog>
#include <QDialogButtonBox>
#include <QSpinBox>
#include <QDoubleSpinBox>
#else
// Simular Qt básico usando console
class QWidget {};
class QMainWindow : public QWidget {};
class QApplication {
public:
    QApplication(int argc, char** argv) {}
    static int exec() { return 0; }
};
#endif

#include "models/produto_simple.h"

class SistemaMainWindow 
#ifdef QT_VERSION
    : public QMainWindow
#endif
{
public:
    SistemaMainWindow() {
        initUI();
    }

    void initUI() {
#ifdef QT_VERSION
        setWindowTitle("Sistema Mercado - Interface Qt");
        setMinimumSize(800, 600);
        
        auto centralWidget = new QWidget();
        setCentralWidget(centralWidget);
        
        auto layout = new QVBoxLayout(centralWidget);
        
        // Header
        auto headerLabel = new QLabel("🏪 Sistema Gerenciador para Supermercados");
        headerLabel->setStyleSheet("font-size: 18px; font-weight: bold; color: #2c3e50; padding: 10px;");
        layout->addWidget(headerLabel);
        
        // Buttons
        auto buttonLayout = new QHBoxLayout();
        
        auto btnProdutos = new QPushButton("📦 Gestão de Produtos");
        auto btnVendas = new QPushButton("💰 Sistema de Vendas");
        auto btnClientes = new QPushButton("👥 Gestão de Clientes");
        auto btnRelatorios = new QPushButton("📊 Relatórios");
        
        buttonLayout->addWidget(btnProdutos);
        buttonLayout->addWidget(btnVendas);
        buttonLayout->addWidget(btnClientes);
        buttonLayout->addWidget(btnRelatorios);
        
        layout->addLayout(buttonLayout);
        
        // Produto table
        produtoTable = new QTableWidget(0, 4);
        QStringList headers = {"Código", "Nome", "Preço", "Estoque"};
        produtoTable->setHorizontalHeaderLabels(headers);
        produtoTable->horizontalHeader()->setStretchLastSection(true);
        layout->addWidget(produtoTable);
        
        // Connect buttons
        connect(btnProdutos, &QPushButton::clicked, [this]() { this->abrirGestaProdutos(); });
        connect(btnVendas, &QPushButton::clicked, [this]() { this->mostrarVendas(); });
        connect(btnClientes, &QPushButton::clicked, [this]() { this->mostrarClientes(); });
        connect(btnRelatorios, &QPushButton::clicked, [this]() { this->mostrarRelatorios(); });
        
        // Add sample data
        addSampleData();
#else
        // Console version
        showConsoleInterface();
#endif
    }

private:
#ifdef QT_VERSION
    QTableWidget* produtoTable;
    std::vector<std::unique_ptr<Produto>> produtos;
    
    void addSampleData() {
        produtos.push_back(std::make_unique<Produto>("PRD001", "Coca-Cola 2L", "Bebidas", 8.50, 45));
        produtos.push_back(std::make_unique<Produto>("PRD002", "Pão Francês", "Padaria", 0.75, 200));
        produtos.push_back(std::make_unique<Produto>("PRD003", "Leite Integral", "Laticínios", 4.20, 30));
        
        updateProductTable();
    }
    
    void updateProductTable() {
        produtoTable->setRowCount(produtos.size());
        
        for (size_t i = 0; i < produtos.size(); ++i) {
            produtoTable->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(produtos[i]->getCodigo())));
            produtoTable->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(produtos[i]->getNome())));
            produtoTable->setItem(i, 2, new QTableWidgetItem(QString::number(produtos[i]->getPreco(), 'f', 2)));
            produtoTable->setItem(i, 3, new QTableWidgetItem(QString::number(produtos[i]->getQuantidadeEstoque())));
        }
    }
    
    void abrirGestaProdutos() {
        QDialog dialog(this);
        dialog.setWindowTitle("Adicionar Produto");
        dialog.setMinimumSize(400, 300);
        
        auto layout = new QFormLayout(&dialog);
        
        auto codigoEdit = new QLineEdit();
        auto nomeEdit = new QLineEdit();
        auto categoriaEdit = new QLineEdit();
        auto precoEdit = new QDoubleSpinBox();
        auto estoqueEdit = new QSpinBox();
        
        precoEdit->setDecimals(2);
        precoEdit->setMaximum(9999.99);
        estoqueEdit->setMaximum(9999);
        
        layout->addRow("Código:", codigoEdit);
        layout->addRow("Nome:", nomeEdit);
        layout->addRow("Categoria:", categoriaEdit);
        layout->addRow("Preço:", precoEdit);
        layout->addRow("Estoque:", estoqueEdit);
        
        auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
        layout->addWidget(buttonBox);
        
        connect(buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
        connect(buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
        
        if (dialog.exec() == QDialog::Accepted) {
            if (!codigoEdit->text().isEmpty() && !nomeEdit->text().isEmpty()) {
                produtos.push_back(std::make_unique<Produto>(
                    codigoEdit->text().toStdString(),
                    nomeEdit->text().toStdString(),
                    categoriaEdit->text().toStdString(),
                    precoEdit->value(),
                    estoqueEdit->value()
                ));
                updateProductTable();
                QMessageBox::information(this, "Sucesso", "Produto adicionado com sucesso!");
            }
        }
    }
    
    void mostrarVendas() {
        QMessageBox::information(this, "Sistema de Vendas", 
            "💰 Sistema de Vendas\n\n"
            "Funcionalidades:\n"
            "• Registro de vendas\n"
            "• Cálculo de totais\n"
            "• Controle de estoque\n"
            "• Histórico de transações");
    }
    
    void mostrarClientes() {
        QMessageBox::information(this, "Gestão de Clientes", 
            "👥 Gestão de Clientes\n\n"
            "Funcionalidades:\n"
            "• Cadastro de clientes\n"
            "• Histórico de compras\n"
            "• Sistema de fidelidade\n"
            "• Relatórios personalizados");
    }
    
    void mostrarRelatorios() {
        QMessageBox::information(this, "Relatórios", 
            "📊 Relatórios Disponíveis\n\n"
            "• Vendas por período\n"
            "• Produtos mais vendidos\n"
            "• Controle de estoque\n"
            "• Análise de clientes\n"
            "• Dashboard interativo");
    }
#else
    void showConsoleInterface() {
        std::cout << "╔══════════════════════════════════════════════════════════════╗" << std::endl;
        std::cout << "║         SISTEMA MERCADO - VERSÃO HÍBRIDA                    ║" << std::endl;
        std::cout << "║         Console + Qt Ready                                   ║" << std::endl;
        std::cout << "╚══════════════════════════════════════════════════════════════╝" << std::endl;
        std::cout << "" << std::endl;
        std::cout << "🎯 Esta versão funciona tanto em console quanto com Qt!" << std::endl;
        std::cout << "📦 Instale Qt5/Qt6 para ver a interface gráfica completa" << std::endl;
        std::cout << "" << std::endl;
        std::cout << "Executando versão console..." << std::endl;
    }
#endif
};

int main(int argc, char *argv[])
{
#ifdef QT_VERSION
    QApplication app(argc, argv);
    
    // Apply modern style
    app.setStyle("Fusion");
    
    SistemaMainWindow window;
    window.show();
    
    return app.exec();
#else
    std::cout << "╔══════════════════════════════════════════════════════════════╗" << std::endl;
    std::cout << "║         SISTEMA MERCADO - VERSÃO HÍBRIDA                    ║" << std::endl;
    std::cout << "╚══════════════════════════════════════════════════════════════╝" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "✅ Compilado sem Qt - Versão console ativa" << std::endl;
    std::cout << "🚀 Para interface gráfica, instale Qt e recompile" << std::endl;
    std::cout << "" << std::endl;
    
    // Criar alguns produtos de exemplo
    Produto p1("PRD001", "Coca-Cola 2L", "Bebidas", 8.50, 45);
    Produto p2("PRD002", "Pão Francês", "Padaria", 0.75, 200);
    Produto p3("PRD003", "Leite Integral", "Laticínios", 4.20, 30);
    
    std::cout << "📦 Produtos cadastrados:" << std::endl;
    std::cout << "   " << p1.toString() << std::endl;
    std::cout << "   " << p2.toString() << std::endl;
    std::cout << "   " << p3.toString() << std::endl;
    std::cout << "" << std::endl;
    std::cout << "🎯 Sistema funcionando! Instale Qt para interface completa." << std::endl;
    
    return 0;
#endif
}
