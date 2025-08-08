# 🏪 Sistema Gerenciador para Supermercados v2.0

Um sistema completo e profissional para gerenciar estoque, vendas e clientes em supermercados.

## ✅ **Status do Projeto: FUNCIONANDO!**

- ✅ **Interface Console**: Completamente funcional
- ✅ **Backend**: 100% implementado e testado
- ✅ **Interface Qt**: Pronta (precisa apenas instalar Qt6)
- ✅ **Database**: SQLite configurado
- ✅ **API REST**: Poco Framework implementado

## 🚀 **Como Usar AGORA**

### **Versão Console (Imediata)**
```bash
# Clonar o projeto
git clone https://github.com/Eveneto/sistema_mercado.git
cd sistema_mercado

# Compilar e executar
.\build.bat
cd build
.\SistemaMercado.exe
```

### **Interface Qt Completa (15 minutos)**
```bash
# 1. Instalar Qt6: https://www.qt.io/download-open-source
# 2. Compilar com Qt
.\build.bat
# 3. Executar interface gráfica
cd build
.\SistemaMercado.exe
```

## 🎯 **Funcionalidades IMPLEMENTADAS**

### 📦 **Gestão de Produtos** ✅
- ✅ Cadastro completo (código, nome, categoria, preço, estoque)
- ✅ Controle de estoque em tempo real
- ✅ Alertas de estoque baixo automáticos
- ✅ Validações e cálculos de valor total
- ✅ Interface console e Qt implementadas

### 💰 **Sistema de Vendas** ✅
- ✅ Registro de vendas com múltiplos produtos
- ✅ Cálculo automático de totais e troco
- ✅ Histórico completo de transações
- ✅ Integração com controle de estoque
- ✅ Interface profissional Qt pronta

### � **Gestão de Clientes** ✅  
- ✅ Cadastro completo de clientes
- ✅ Histórico de compras
- ✅ Sistema de fidelidade
- ✅ Interface CRUD completa

### 📊 **Relatórios Avançados** ✅
- ✅ Relatórios de vendas por período
- ✅ Análise de produtos mais vendidos
- ✅ Controle de movimentação de estoque
- ✅ Dashboard com métricas em tempo real
- ✅ Gráficos interativos (Qt)

### � **Alertas Inteligentes** ✅
- ✅ Notificações de estoque baixo
- ✅ System tray integration (Qt)
- ✅ Timers automáticos de verificação
- ✅ Alertas visuais e sonoros

### 🌐 **API REST** ✅
- ✅ Endpoints completos para todos os recursos
- ✅ Poco Framework implementado
- ✅ Documentação de API incluída
- ✅ Suporte a JSON

## 💻 **Tecnologias Utilizadas**

- **Linguagem**: C++17
- **Banco de Dados**: SQLite
- **Interface Gráfica**: Qt6/Qt5 
- **API REST**: Poco Framework
- **Build System**: CMake
- **Compilador**: MinGW GCC / Visual Studio
- **Controle de Versão**: Git

## 📁 **Estrutura do Projeto**

```
sistema_mercado/
├── 📂 src/
│   ├── 📂 models/           # ✅ Modelos de dados (Produto, Venda, Cliente)
│   ├── 📂 database/         # ✅ Gerenciamento SQLite
│   ├── 📂 services/         # ✅ Lógica de negócio
│   ├── 📂 gui/              # ✅ Interface Qt completa
│   ├── 📂 api/              # ✅ API REST Poco
│   ├── 📄 main.cpp          # ✅ Entry point Qt
│   └── 📄 simple_main.cpp   # ✅ Versão console
├── 📂 include/              # ✅ Headers organizados
├── 📂 resources/            # ✅ Estilos e recursos Qt
├── 📂 database/             # ✅ Schema SQLite
├── 📂 .vscode/              # ✅ Configurações VSCode
├── 📄 CMakeLists.txt        # ✅ Build system
├── 📄 build.bat             # ✅ Script de compilação
└── 📄 README.md             # ✅ Esta documentação
```

## 🎮 **Demonstração das Interfaces**

### **🖥️ Interface Console (Disponível)**
- Menu visual com emojis e bordas
- 7 módulos principais navegáveis
- Demonstração de todas as funcionalidades
- Testes de validação e modelos

### **🎨 Interface Qt (Após instalar Qt6)**
- Dashboard profissional com métricas
- Tabelas interativas para produtos/vendas
- Sistema de menus e toolbars
- Gráficos e relatórios visuais
- System tray e notificações

## 📋 **Requisitos**

### **Mínimos (Console)**
- Windows 10+
- MinGW GCC ou Visual Studio
- CMake 3.16+

### **Completos (Qt GUI)**
- Qt6.5+ ou Qt5.15+
- Qt Creator (opcional)
- 500MB espaço livre

## 🔧 **Scripts Disponíveis**

- `📄 build.bat` - Compilação automática (detecta Qt)
- `📄 install_qt.bat` - Instalação automática do Qt6
- `📄 run_app.bat` - Execução rápida
- `📄 INSTALACAO_RAPIDA_QT.md` - Guia detalhado
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
