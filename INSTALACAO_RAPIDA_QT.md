# 🚀 GUIA RÁPIDO: Instalação do Qt6

## 📋 **O que você precisa:**
1. **Qt6 instalado** 
2. **5 minutos do seu tempo**

## ⚡ **Método Mais Rápido (Recomendado)**

### **Passo 1: Baixar Qt6**
1. Acesse: **https://www.qt.io/download-open-source**
2. Clique em: **"Download Qt Online Installer"**
3. Baixe o arquivo: `qt-unified-windows-x64-online.exe`

### **Passo 2: Instalar Qt6**
1. **Execute** o instalador como **Administrador**
2. **Crie conta** Qt (gratuita)
3. **Selecione componentes:**
   - ✅ Qt 6.5+ LTS 
   - ✅ Qt Creator
   - ✅ CMake
   - ✅ MinGW 11.2 (64-bit)
4. **Instalar em:** `C:\Qt`

### **Passo 3: Rodar Sistema**
```bash
# Voltar ao diretório do projeto
cd d:\sistema_mercado

# Compilar versão Qt
.\build.bat

# Executar interface gráfica
cd build
.\SistemaMercado.exe
```

## 🎯 **Resultado Final:**

Você terá uma **interface gráfica profissional** com:

- 📊 **Dashboard** interativo
- 🛒 **Gestão de produtos** com tabelas  
- 💰 **Sistema de vendas** intuitivo
- 👥 **Cadastro de clientes**
- 📈 **Relatórios** com gráficos
- 🔔 **Alertas** automáticos
- 🎨 **Design moderno** e responsivo

## ⏱️ **Tempo estimado:** 15 minutos

## 🆘 **Problemas?**

### Qt6 não encontrado após instalação:
```bash
# Adicionar ao PATH manualmente
$env:PATH += ";C:\Qt\6.5.0\mingw_64\bin"
$env:CMAKE_PREFIX_PATH = "C:\Qt\6.5.0\mingw_64"
```

### Erro de compilação:
```bash
# Limpar e recompilar
cd d:\sistema_mercado
rmdir build -Recurse -Force
.\build.bat
```

## 🎉 **Está Funcionando o Console?**
Se quiser testar agora:
```bash
cd d:\sistema_mercado\build
.\SistemaMercado.exe
```

**A interface Qt é 10x mais poderosa que a versão console!** 🚀
