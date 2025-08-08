#include "database/database_manager.h"
#include <iostream>
#include <sstream>
#include <QDir>
#include <QStandardPaths>

DatabaseManager::DatabaseManager(const std::string& db_path) 
    : db(nullptr), db_path(db_path), connected(false) {
    
    // Criar diretório do banco se não existir
    QDir dir;
    QString qdb_path = QString::fromStdString(db_path);
    QString db_dir = QFileInfo(qdb_path).absolutePath();
    if (!dir.exists(db_dir)) {
        dir.mkpath(db_dir);
    }
}

DatabaseManager::~DatabaseManager() {
    desconectar();
}

bool DatabaseManager::conectar() {
    if (connected) {
        return true;
    }
    
    int rc = sqlite3_open(db_path.c_str(), &db);
    if (rc != SQLITE_OK) {
        std::cerr << "Erro ao abrir banco: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        db = nullptr;
        return false;
    }
    
    // Habilitar foreign keys
    executarSQL("PRAGMA foreign_keys = ON;");
    
    connected = true;
    return true;
}

void DatabaseManager::desconectar() {
    if (db) {
        sqlite3_close(db);
        db = nullptr;
    }
    connected = false;
}

bool DatabaseManager::inicializarBanco() {
    if (!conectar()) {
        return false;
    }
    
    return criarTabelas();
}

bool DatabaseManager::criarTabelas() {
    return criarTabelaProdutos() && 
           criarTabelaClientes() && 
           criarTabelaVendas() && 
           criarTabelaItensVenda() &&
           criarIndices();
}

bool DatabaseManager::criarTabelaProdutos() {
    const char* sql = R"(
        CREATE TABLE IF NOT EXISTS produtos (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            codigo TEXT UNIQUE NOT NULL,
            nome TEXT NOT NULL,
            categoria TEXT,
            preco REAL NOT NULL DEFAULT 0,
            quantidade_estoque INTEGER NOT NULL DEFAULT 0,
            estoque_minimo INTEGER NOT NULL DEFAULT 10,
            data_criacao DATETIME DEFAULT CURRENT_TIMESTAMP,
            data_vencimento DATETIME,
            ativo BOOLEAN DEFAULT 1
        );
    )";
    
    return executarSQL(sql);
}

bool DatabaseManager::criarTabelaClientes() {
    const char* sql = R"(
        CREATE TABLE IF NOT EXISTS clientes (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            nome TEXT NOT NULL,
            cpf TEXT UNIQUE,
            telefone TEXT,
            email TEXT,
            endereco TEXT,
            data_cadastro DATETIME DEFAULT CURRENT_TIMESTAMP,
            ativo BOOLEAN DEFAULT 1,
            limite_credito REAL DEFAULT 0,
            saldo_devedor REAL DEFAULT 0
        );
    )";
    
    return executarSQL(sql);
}

bool DatabaseManager::criarTabelaVendas() {
    const char* sql = R"(
        CREATE TABLE IF NOT EXISTS vendas (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            data_venda DATETIME DEFAULT CURRENT_TIMESTAMP,
            total REAL NOT NULL DEFAULT 0,
            forma_pagamento INTEGER NOT NULL DEFAULT 0,
            valor_pago REAL DEFAULT 0,
            troco REAL DEFAULT 0,
            observacoes TEXT,
            cliente_id INTEGER,
            cancelada BOOLEAN DEFAULT 0,
            FOREIGN KEY (cliente_id) REFERENCES clientes(id)
        );
    )";
    
    return executarSQL(sql);
}

bool DatabaseManager::criarTabelaItensVenda() {
    const char* sql = R"(
        CREATE TABLE IF NOT EXISTS itens_venda (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            venda_id INTEGER NOT NULL,
            produto_id INTEGER NOT NULL,
            quantidade INTEGER NOT NULL,
            preco_unitario REAL NOT NULL,
            subtotal REAL NOT NULL,
            FOREIGN KEY (venda_id) REFERENCES vendas(id) ON DELETE CASCADE,
            FOREIGN KEY (produto_id) REFERENCES produtos(id)
        );
    )";
    
    return executarSQL(sql);
}

bool DatabaseManager::criarIndices() {
    std::vector<std::string> indices = {
        "CREATE INDEX IF NOT EXISTS idx_produtos_codigo ON produtos(codigo);",
        "CREATE INDEX IF NOT EXISTS idx_produtos_nome ON produtos(nome);",
        "CREATE INDEX IF NOT EXISTS idx_produtos_categoria ON produtos(categoria);",
        "CREATE INDEX IF NOT EXISTS idx_clientes_cpf ON clientes(cpf);",
        "CREATE INDEX IF NOT EXISTS idx_clientes_nome ON clientes(nome);",
        "CREATE INDEX IF NOT EXISTS idx_vendas_data ON vendas(data_venda);",
        "CREATE INDEX IF NOT EXISTS idx_itens_venda_produto ON itens_venda(produto_id);",
        "CREATE INDEX IF NOT EXISTS idx_itens_venda_venda ON itens_venda(venda_id);"
    };
    
    for (const auto& sql : indices) {
        if (!executarSQL(sql)) {
            return false;
        }
    }
    
    return true;
}

bool DatabaseManager::executarSQL(const std::string& sql) {
    if (!connected || !db) {
        return false;
    }
    
    char* erro = nullptr;
    int rc = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &erro);
    
    if (rc != SQLITE_OK) {
        std::cerr << "Erro SQL: " << erro << std::endl;
        sqlite3_free(erro);
        return false;
    }
    
    return true;
}

int DatabaseManager::getLastInsertId() {
    return sqlite3_last_insert_rowid(db);
}

bool DatabaseManager::inserirProduto(Produto& produto) {
    if (!connected || !db) {
        return false;
    }
    
    const char* sql = R"(
        INSERT INTO produtos (codigo, nome, categoria, preco, quantidade_estoque, 
                            estoque_minimo, data_vencimento, ativo)
        VALUES (?, ?, ?, ?, ?, ?, ?, ?);
    )";
    
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        return false;
    }
    
    sqlite3_bind_text(stmt, 1, produto.getCodigo().c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, produto.getNome().c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, produto.getCategoria().c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_double(stmt, 4, produto.getPreco());
    sqlite3_bind_int(stmt, 5, produto.getQuantidadeEstoque());
    sqlite3_bind_int(stmt, 6, produto.getEstoqueMinimo());
    
    if (produto.getDataVencimento().isValid()) {
        sqlite3_bind_text(stmt, 7, produto.getDataVencimento().toString(Qt::ISODate).toStdString().c_str(), -1, SQLITE_STATIC);
    } else {
        sqlite3_bind_null(stmt, 7);
    }
    
    sqlite3_bind_int(stmt, 8, produto.isAtivo() ? 1 : 0);
    
    bool sucesso = sqlite3_step(stmt) == SQLITE_DONE;
    
    if (sucesso) {
        produto.setId(getLastInsertId());
    }
    
    sqlite3_finalize(stmt);
    return sucesso;
}

std::unique_ptr<Produto> DatabaseManager::buscarProduto(int id) {
    if (!connected || !db) {
        return nullptr;
    }
    
    const char* sql = R"(
        SELECT id, codigo, nome, categoria, preco, quantidade_estoque, 
               estoque_minimo, data_criacao, data_vencimento, ativo
        FROM produtos WHERE id = ?;
    )";
    
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        return nullptr;
    }
    
    sqlite3_bind_int(stmt, 1, id);
    
    std::unique_ptr<Produto> produto = nullptr;
    
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        produto = std::make_unique<Produto>();
        
        produto->setId(sqlite3_column_int(stmt, 0));
        produto->setCodigo(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
        produto->setNome(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)));
        
        const char* categoria = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        if (categoria) produto->setCategoria(categoria);
        
        produto->setPreco(sqlite3_column_double(stmt, 4));
        produto->setQuantidadeEstoque(sqlite3_column_int(stmt, 5));
        produto->setEstoqueMinimo(sqlite3_column_int(stmt, 6));
        
        const char* data_vencimento = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 8));
        if (data_vencimento) {
            produto->setDataVencimento(QDateTime::fromString(QString::fromUtf8(data_vencimento), Qt::ISODate));
        }
        
        produto->setAtivo(sqlite3_column_int(stmt, 9) == 1);
    }
    
    sqlite3_finalize(stmt);
    return produto;
}

std::string DatabaseManager::getUltimoErro() const {
    if (db) {
        return sqlite3_errmsg(db);
    }
    return "Banco não conectado";
}
