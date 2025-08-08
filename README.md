# Sistema Gerenciador para Supermercados

Um sistema completo para gerenciar estoque, vendas e clientes em pequenos supermercados.

## Tecnologias Utilizadas

- **Backend**: C++17
- **Banco de Dados**: SQLite
- **Interface Gráfica**: Qt6
- **API REST**: Poco Framework
- **Build System**: CMake

## Funcionalidades

### 📦 Gestão de Produtos
- Cadastro de produtos (nome, código, preço, quantidade)
- Controle de estoque em tempo real
- Alertas de estoque baixo
- Categorização de produtos

### 💰 Sistema de Vendas
- Registro de vendas com múltiplos produtos
- Cálculo automático de total e troco
- Histórico completo de transações
- Suporte a diferentes formas de pagamento

### 📊 Relatórios
- Relatórios diários de vendas
- Relatórios mensais de faturamento
- Análise de produtos mais vendidos
- Controle de movimentação de estoque

### 🚨 Alertas Inteligentes
- Notificações de estoque baixo
- Alertas de produtos próximos ao vencimento
- Resumos de vendas diárias

## Estrutura do Projeto

```
sistema_mercado/
├── src/
│   ├── models/           # Modelos de dados
│   ├── database/         # Gerenciamento do banco
│   ├── services/         # Lógica de negócio
│   ├── gui/              # Interface gráfica Qt
│   └── api/              # API REST
├── include/              # Arquivos de cabeçalho
├── resources/            # Recursos (ícones, etc.)
├── database/             # Arquivo SQLite
└── build/                # Arquivos de compilação
```

## Pré-requisitos

### Windows
- Visual Studio 2019 ou superior
- Qt6 (6.2 ou superior)
- CMake 3.16 ou superior
- Poco C++ Libraries
- SQLite3

### Instalação das Dependências

#### Qt6
1. Baixe o Qt Online Installer
2. Instale Qt 6.5 LTS ou superior
3. Adicione o Qt ao PATH do sistema

#### Poco
```powershell
# Usando vcpkg (recomendado)
vcpkg install poco[complete]:x64-windows
```

#### CMake
```powershell
# Usando Chocolatey
choco install cmake
```

## Compilação

1. Clone o repositório:
```bash
git clone <url-do-repositorio>
cd sistema_mercado
```

2. Configure o projeto:
```bash
mkdir build
cd build
cmake .. -DCMAKE_PREFIX_PATH="C:/Qt/6.5.0/msvc2019_64"
```

3. Compile:
```bash
cmake --build . --config Release
```

## Uso

### Interface Desktop
Execute o arquivo `SistemaMercado.exe` gerado na pasta build.

### API REST
A API estará disponível em `http://localhost:8080` com os seguintes endpoints:

- `GET /api/produtos` - Lista todos os produtos
- `POST /api/produtos` - Cadastra novo produto
- `GET /api/vendas` - Lista todas as vendas
- `POST /api/vendas` - Registra nova venda
- `GET /api/relatorios/vendas` - Relatório de vendas

## Banco de Dados

O sistema utiliza SQLite com as seguintes tabelas:

- **produtos**: Informações dos produtos
- **vendas**: Registro de vendas
- **itens_venda**: Itens de cada venda
- **clientes**: Dados dos clientes (opcional)

## Casos de Uso

### Pequeno Mercado Local
- Controle de estoque de produtos perecíveis
- Registro rápido de vendas
- Geração de relatórios para tomada de decisão
- Alertas para reposição de produtos

### Minimercado Familiar
- Interface simples e intuitiva
- Relatórios de faturamento
- Controle de produtos em falta

## Contribuição

1. Fork o projeto
2. Crie uma branch para sua feature
3. Commit suas mudanças
4. Push para a branch
5. Abra um Pull Request

## Licença

Este projeto está sob a licença MIT. Veja o arquivo LICENSE para mais detalhes.

## Suporte

Para suporte e dúvidas, abra uma issue no repositório do projeto.
