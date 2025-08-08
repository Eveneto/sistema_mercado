-- Script de inicialização do banco de dados
-- Sistema Gerenciador para Supermercados

-- Inserir categorias de produtos (via produtos)
INSERT OR IGNORE INTO produtos (codigo, nome, categoria, preco, quantidade_estoque, estoque_minimo, ativo) VALUES
-- Bebidas
('PRD0001', 'Coca-Cola 2L', 'Bebidas', 8.50, 50, 10, 1),
('PRD0002', 'Água Mineral 500ml', 'Bebidas', 2.00, 100, 20, 1),
('PRD0003', 'Suco de Laranja 1L', 'Bebidas', 6.90, 30, 5, 1),
('PRD0004', 'Cerveja Skol 350ml', 'Bebidas', 3.50, 80, 15, 1),

-- Laticínios
('PRD0005', 'Leite Integral 1L', 'Laticínios', 4.20, 40, 8, 1),
('PRD0006', 'Queijo Mussarela 500g', 'Laticínios', 18.90, 25, 5, 1),
('PRD0007', 'Iogurte Natural 170g', 'Laticínios', 3.80, 60, 12, 1),
('PRD0008', 'Manteiga 200g', 'Laticínios', 7.50, 35, 8, 1),

-- Padaria
('PRD0009', 'Pão de Forma Integral', 'Padaria', 5.20, 45, 10, 1),
('PRD0010', 'Pão Francês (kg)', 'Padaria', 8.90, 20, 5, 1),
('PRD0011', 'Bolo de Chocolate', 'Padaria', 15.50, 12, 3, 1),

-- Higiene
('PRD0012', 'Sabonete Dove 90g', 'Higiene', 4.90, 80, 15, 1),
('PRD0013', 'Shampoo Seda 325ml', 'Higiene', 12.90, 40, 8, 1),
('PRD0014', 'Creme Dental Colgate 90g', 'Higiene', 6.50, 60, 12, 1),
('PRD0015', 'Papel Higiênico 4 rolos', 'Higiene', 8.90, 50, 10, 1),

-- Limpeza
('PRD0016', 'Detergente Ypê 500ml', 'Limpeza', 2.80, 70, 15, 1),
('PRD0017', 'Desinfetante Pinho Sol 1L', 'Limpeza', 7.90, 35, 8, 1),
('PRD0018', 'Amaciante Comfort 2L', 'Limpeza', 14.50, 25, 5, 1),

-- Mercearia
('PRD0019', 'Arroz Branco 5kg', 'Mercearia', 22.90, 30, 5, 1),
('PRD0020', 'Feijão Preto 1kg', 'Mercearia', 8.50, 40, 8, 1),
('PRD0021', 'Açúcar Cristal 1kg', 'Mercearia', 5.20, 50, 10, 1),
('PRD0022', 'Óleo de Soja 900ml', 'Mercearia', 6.90, 45, 10, 1),
('PRD0023', 'Macarrão Espaguete 500g', 'Mercearia', 4.50, 60, 12, 1),

-- Produtos com estoque baixo (para testar alertas)
('PRD0024', 'Farinha de Trigo 1kg', 'Mercearia', 4.80, 3, 10, 1),
('PRD0025', 'Café em Pó 500g', 'Mercearia', 12.90, 2, 8, 1);

-- Inserir alguns clientes de exemplo
INSERT OR IGNORE INTO clientes (nome, cpf, telefone, email, ativo, limite_credito) VALUES
('João Silva', '12345678901', '(11) 99999-1234', 'joao@email.com', 1, 500.00),
('Maria Santos', '98765432109', '(11) 88888-5678', 'maria@email.com', 1, 1000.00),
('Pedro Oliveira', '45678912345', '(11) 77777-9012', 'pedro@email.com', 1, 300.00),
('Ana Costa', '78912345678', '(11) 66666-3456', 'ana@email.com', 1, 750.00),
('Cliente Balcão', '', '', '', 1, 0.00);

-- Inserir algumas vendas de exemplo
INSERT OR IGNORE INTO vendas (data_venda, total, forma_pagamento, valor_pago, troco, cliente_id, cancelada) VALUES
(datetime('now', '-2 days'), 45.80, 0, 50.00, 4.20, 1, 0),
(datetime('now', '-2 days'), 23.50, 1, 23.50, 0.00, 2, 0),
(datetime('now', '-1 day'), 78.90, 0, 80.00, 1.10, 3, 0),
(datetime('now', '-1 day'), 156.70, 2, 156.70, 0.00, 4, 0),
(datetime('now'), 34.20, 0, 35.00, 0.80, 5, 0);

-- Inserir itens das vendas de exemplo
INSERT OR IGNORE INTO itens_venda (venda_id, produto_id, quantidade, preco_unitario, subtotal) VALUES
-- Venda 1 (ID 1)
(1, 1, 2, 8.50, 17.00),
(1, 5, 3, 4.20, 12.60),
(1, 12, 2, 4.90, 9.80),
(1, 16, 2, 2.80, 5.60),

-- Venda 2 (ID 2)
(2, 2, 5, 2.00, 10.00),
(2, 14, 2, 6.50, 13.00),

-- Venda 3 (ID 3)
(3, 19, 1, 22.90, 22.90),
(3, 20, 2, 8.50, 17.00),
(3, 21, 3, 5.20, 15.60),
(3, 22, 2, 6.90, 13.80),
(3, 23, 2, 4.50, 9.00),

-- Venda 4 (ID 4)
(4, 6, 3, 18.90, 56.70),
(4, 8, 4, 7.50, 30.00),
(4, 11, 2, 15.50, 31.00),
(4, 13, 3, 12.90, 38.70),

-- Venda 5 (ID 5)
(5, 3, 2, 6.90, 13.80),
(5, 7, 4, 3.80, 15.20),
(5, 9, 1, 5.20, 5.20);

-- Configurações do sistema (tabela adicional para configurações)
CREATE TABLE IF NOT EXISTS configuracoes (
    chave TEXT PRIMARY KEY,
    valor TEXT NOT NULL,
    descricao TEXT,
    data_atualizacao DATETIME DEFAULT CURRENT_TIMESTAMP
);

INSERT OR IGNORE INTO configuracoes (chave, valor, descricao) VALUES
('nome_empresa', 'Supermercado Exemplo', 'Nome da empresa'),
('cnpj_empresa', '12.345.678/0001-90', 'CNPJ da empresa'),
('endereco_empresa', 'Rua das Flores, 123 - Centro', 'Endereço da empresa'),
('telefone_empresa', '(11) 3333-4444', 'Telefone da empresa'),
('versao_sistema', '1.0.0', 'Versão atual do sistema'),
('backup_automatico', '1', 'Backup automático habilitado (1) ou desabilitado (0)'),
('alerta_estoque_baixo', '1', 'Alertas de estoque baixo habilitados'),
('email_alertas', 'admin@supermercado.com', 'Email para envio de alertas');

-- Criar view para relatórios rápidos
CREATE VIEW IF NOT EXISTS view_produtos_estoque_baixo AS
SELECT 
    p.id,
    p.codigo,
    p.nome,
    p.categoria,
    p.quantidade_estoque,
    p.estoque_minimo,
    p.preco,
    (p.quantidade_estoque * p.preco) as valor_estoque
FROM produtos p
WHERE p.ativo = 1 
AND p.quantidade_estoque <= p.estoque_minimo
ORDER BY p.quantidade_estoque ASC;

-- Criar view para vendas do dia
CREATE VIEW IF NOT EXISTS view_vendas_hoje AS
SELECT 
    v.id,
    v.data_venda,
    v.total,
    v.forma_pagamento,
    c.nome as cliente_nome,
    COUNT(iv.id) as quantidade_itens
FROM vendas v
LEFT JOIN clientes c ON v.cliente_id = c.id
LEFT JOIN itens_venda iv ON v.id = iv.venda_id
WHERE DATE(v.data_venda) = DATE('now')
AND v.cancelada = 0
GROUP BY v.id
ORDER BY v.data_venda DESC;

-- Índices adicionais para performance
CREATE INDEX IF NOT EXISTS idx_vendas_data_cancelada ON vendas(data_venda, cancelada);
CREATE INDEX IF NOT EXISTS idx_produtos_categoria_ativo ON produtos(categoria, ativo);
CREATE INDEX IF NOT EXISTS idx_itens_venda_produto_venda ON itens_venda(produto_id, venda_id);
