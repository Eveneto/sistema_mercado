# 🎯 GUIA DEFINITIVO: Instalação Qt6 Automática

## ✨ **Baseado na Documentação Oficial Qt6**
Fonte: https://doc.qt.io/qt-6/get-and-install-qt-cli.html

## 🚀 **3 Métodos de Instalação**

### **Método 1: Script Automático (Recomendado)** ⚡
```bash
# PowerShell (Mais robusto)
.\install_qt6_fixed.ps1

# Ou Batch (Mais simples)
.\install_qt6_auto.bat
```

### **Método 2: Manual via Linha de Comando** 🔧
```bash
# 1. Baixar instalador
curl -O https://download.qt.io/official_releases/online_installers/qt-unified-windows-x64-online.exe

# 2. Instalação automática
qt-unified-windows-x64-online.exe --root C:\Qt --accept-licenses --accept-obligations --default-answer --confirm-command --auto-answer telemetry-question=No --auto-answer AssociateCommonFiletypes=Yes install qt.tools.qtcreator qt.tools.cmake qt.tools.mingw1120 qt.qt6.681.win64_mingw
```

### **Método 3: Interface Gráfica** 🖱️
1. Baixar: https://www.qt.io/download-open-source
2. Executar instalador
3. Selecionar componentes

## 📦 **O que será instalado:**

| Componente | Descrição |
|------------|-----------|
| `qt.qt6.681.win64_mingw` | Qt 6.8.1 com MinGW |
| `qt.tools.qtcreator` | Qt Creator IDE |
| `qt.tools.cmake` | CMake integration |
| `qt.tools.mingw1120` | MinGW 11.2 compiler |

## 🔧 **Após a Instalação:**

### **1. Verificar instalação**
```bash
# Verificar se Qt foi instalado
dir C:\Qt

# Verificar versões
C:\Qt\6.8.1\mingw_64\bin\qmake.exe -version
```

### **2. Compilar projeto**
```bash
# Navegar para projeto
cd d:\sistema_mercado

# Compilar (detecta Qt automaticamente)
.\build.bat

# Executar interface Qt
cd build
.\SistemaMercado.exe
```

## 🎯 **Comandos da Documentação Oficial**

### **Buscar pacotes disponíveis:**
```bash
qt-unified-windows-x64-online.exe search
```

### **Instalar componentes específicos:**
```bash
# Qt 6.8.1 essentials
qt-unified-windows-x64-online.exe install qt6.681-essentials

# Qt 6.8.1 completo
qt-unified-windows-x64-online.exe install qt6.681-full

# Qt 6.8.1 SDK (recomendado)
qt-unified-windows-x64-online.exe install qt6.681-sdk
```

### **Listar pacotes instalados:**
```bash
MaintenanceTool.exe list
```

### **Atualizar Qt:**
```bash
MaintenanceTool.exe update
```

## 🔧 **Troubleshooting**

### **Qt não detectado após instalação:**
```bash
# Adicionar ao PATH manualmente
$env:PATH += ";C:\Qt\6.8.1\mingw_64\bin"
$env:CMAKE_PREFIX_PATH = "C:\Qt\6.8.1\mingw_64\lib\cmake"
```

### **Erro de permissões:**
- Execute PowerShell como Administrador
- Ou instale em diretório do usuário: `--root C:\Users\%USERNAME%\Qt`

### **Instalação interativa se automática falhar:**
```bash
qt-unified-windows-x64-online.exe --root C:\Qt install qt.qt6.681.win64_mingw
```

## 📋 **Variações por Versão Qt**

| Versão | Package Name | Recomendação |
|--------|-------------|--------------|
| Qt 6.8.1 | `qt.qt6.681.win64_mingw` | ✅ LTS Estável |
| Qt 6.9.1 | `qt.qt6.691.win64_mingw` | 🔄 Mais recente |
| Qt 6.5.3 | `qt.qt6.653.win64_mingw` | 🔒 LTS Anterior |

## 🎉 **Resultado Final**

Após instalação bem-sucedida:
- ✅ Qt6 em `C:\Qt\`
- ✅ Qt Creator disponível
- ✅ MinGW compiler configurado
- ✅ CMake integration ativa
- ✅ Build automático funcionando
- ✅ Interface gráfica Qt rodando!

## 📞 **Suporte**

- 📚 Documentação: https://doc.qt.io/qt-6/
- 🐛 Issues: https://github.com/Eveneto/sistema_mercado/issues
- 💬 Comunidade: https://forum.qt.io/

**🎯 Com estes scripts, a instalação do Qt6 fica 100% automatizada!** 🚀
