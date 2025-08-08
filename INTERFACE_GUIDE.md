# 🚀 Como Rodar a Interface Completa do Sistema Mercado

## 📋 **Opções Disponíveis**

### ⚡ **OPÇÃO 1: Interface Simplificada (FUNCIONA AGORA)**
```batch
# Execute imediatamente:
run_app.bat
```
- ✅ **Funciona agora** - Console com menu interativo
- ✅ **Todas as funcionalidades** principais
- ✅ **Não requer Qt** - usa apenas C++ padrão

### 🎨 **OPÇÃO 2: Interface Gráfica Completa (Qt6)**

#### **2A. Instalação Automática**
```powershell
# Execute como administrador:
.\install_qt.ps1
```

#### **2B. Instalação Manual**
1. **Baixar Qt:**
   - Acesse: https://www.qt.io/download-qt-installer
   - Baixe: `qt-unified-windows-x64-online.exe`

2. **Instalar Qt6:**
   ```
   Componentes necessários:
   ☑ Qt 6.5.x Desktop (MinGW 64-bit)
   ☑ Qt Creator
   ☑ CMake
   ☑ Qt Quick Designer Tools
   ```

3. **Configurar PATH:**
   ```powershell
   # Adicionar ao PATH do sistema:
   C:\Qt\6.5.x\mingw_64\bin
   C:\Qt\Tools\mingw_64\bin
   C:\Qt\Tools\CMake_64\bin
   ```

4. **Compilar com Qt:**
   ```batch
   mkdir build_qt
   cd build_qt
   cmake .. -DCMAKE_PREFIX_PATH=C:\Qt\6.5.x\mingw_64
   mingw32-make
   ```

#### **2C. Usando vcpkg (Alternativa)**
```powershell
# Instalar vcpkg
git clone https://github.com/Microsoft/vcpkg.git C:\tools\vcpkg
cd C:\tools\vcpkg
.\bootstrap-vcpkg.bat

# Instalar Qt6
.\vcpkg install qt6-base:x64-windows
.\vcpkg install qt6-widgets:x64-windows

# Compilar
cmake .. -DCMAKE_TOOLCHAIN_FILE=C:\tools\vcpkg\scripts\buildsystems\vcpkg.cmake
```

## 🎯 **Funcionalidades por Interface**

### 📟 **Console (Disponível Agora)**
```
Sistema Gerenciador de Supermercados
====================================
1. Gerenciar Produtos
2. Processar Vendas  
3. Cadastrar Clientes
4. Relatórios
5. Configurações
0. Sair
```

### 🖥 **Qt GUI (Após Instalar Qt)**
```
┌─ Sistema Mercado ─────────────────────────┐
│ Arquivo | Cadastros | Vendas | Relatórios │
├───────────────────────────────────────────┤
│ [Nova Venda] [Produtos] [Clientes]        │
├───────────────────────────────────────────┤
│ 📊 Dashboard                              │
│ ├─ Vendas Hoje: R$ 1.250,00              │
│ ├─ Produtos: 150 itens                   │
│ └─ ⚠ Estoque Baixo: 5 produtos           │
│                                           │
│ 🛒 Ações Rápidas                         │
│ [Nova Venda] [Cadastrar Produto]         │
│ [Ver Relatórios] [Backup]                │
└───────────────────────────────────────────┘
```

## 🔧 **Solução de Problemas**

### ❌ **Qt não encontrado**
```bash
# Verificar instalação:
qmake --version
cmake --version

# Se não funcionar, use:
run_app.bat  # Interface console
```

### ❌ **Erro de compilação**
```bash
# Recompilar limpo:
rmdir /s build
build_simple.bat
```

### ❌ **DLL não encontrada**
```bash
# Copiar DLLs Qt (após instalar Qt):
copy C:\Qt\6.5.x\mingw_64\bin\*.dll build\
```

## 📱 **Próximos Passos**

1. **Teste Imediato:**
   ```batch
   run_app.bat
   ```

2. **Para Interface Gráfica:**
   - Instale Qt6 seguindo opção 2B
   - Recompile com suporte Qt
   - Aproveite a interface completa!

## 🎨 **Recursos da Interface Qt**

✅ **Dashboard Interativo** com métricas em tempo real  
✅ **Menus Contextuais** e atalhos de teclado  
✅ **Tabelas Editáveis** para produtos e vendas  
✅ **Dialogs Modais** para cadastros  
✅ **Sistema de Alertas** automático  
✅ **Themes Personalizáveis** (Dark/Light)  
✅ **Sistema Tray** (minimizar para bandeja)  
✅ **Relatórios Visuais** com gráficos  
✅ **Backup/Restore** integrado  

---
*Execute `run_app.bat` para testar agora mesmo!* 🚀
