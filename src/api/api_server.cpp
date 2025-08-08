#include "api/api_server.h"
#include "services/produto_service.h"
#include "services/venda_service.h"
#include "database/database_manager.h"
#include <Poco/Net/HTTPServerParams.h>
#include <Poco/URI.h>
#include <Poco/JSON/Parser.h>
#include <Poco/StreamCopier.h>
#include <iostream>
#include <sstream>

// ProdutoHandler Implementation
ProdutoHandler::ProdutoHandler(ProdutoService* service) 
    : produto_service(service) {
}

void ProdutoHandler::handleRequest(Poco::Net::HTTPServerRequest& request, 
                                  Poco::Net::HTTPServerResponse& response) {
    response.setContentType("application/json");
    response.set("Access-Control-Allow-Origin", "*");
    response.set("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
    response.set("Access-Control-Allow-Headers", "Content-Type, Authorization");
    
    try {
        std::string method = request.getMethod();
        
        if (method == "GET") {
            handleGet(request, response);
        } else if (method == "POST") {
            handlePost(request, response);
        } else if (method == "PUT") {
            handlePut(request, response);
        } else if (method == "DELETE") {
            handleDelete(request, response);
        } else if (method == "OPTIONS") {
            response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
            response.send();
        } else {
            auto error = ApiServer::criarRespostaErro("Método não suportado", 405);
            ApiServer::enviarResposta(response, error, 405);
        }
    } catch (const std::exception& e) {
        auto error = ApiServer::criarRespostaErro(e.what(), 500);
        ApiServer::enviarResposta(response, error, 500);
    }
}

void ProdutoHandler::handleGet(Poco::Net::HTTPServerRequest& request, 
                              Poco::Net::HTTPServerResponse& response) {
    Poco::URI uri(request.getURI());
    std::string path = uri.getPath();
    
    // Extrair ID do produto se fornecido (/api/produtos/123)
    if (path.length() > 14) { // "/api/produtos/".length() = 14
        std::string id_str = path.substr(14);
        try {
            int id = std::stoi(id_str);
            auto produto = produto_service->buscarProduto(id);
            
            if (produto) {
                Poco::JSON::Object::Ptr json = new Poco::JSON::Object;
                json->set("id", produto->getId());
                json->set("codigo", produto->getCodigo());
                json->set("nome", produto->getNome());
                json->set("categoria", produto->getCategoria());
                json->set("preco", produto->getPreco());
                json->set("quantidade_estoque", produto->getQuantidadeEstoque());
                json->set("estoque_minimo", produto->getEstoqueMinimo());
                json->set("ativo", produto->isAtivo());
                
                ApiServer::enviarResposta(response, json);
            } else {
                auto error = ApiServer::criarRespostaErro("Produto não encontrado", 404);
                ApiServer::enviarResposta(response, error, 404);
            }
        } catch (...) {
            auto error = ApiServer::criarRespostaErro("ID inválido", 400);
            ApiServer::enviarResposta(response, error, 400);
        }
    } else {
        // Listar todos os produtos
        auto produtos = produto_service->listarProdutosAtivos();
        
        Poco::JSON::Array::Ptr array = new Poco::JSON::Array;
        for (const auto& produto : produtos) {
            Poco::JSON::Object::Ptr obj = new Poco::JSON::Object;
            obj->set("id", produto.getId());
            obj->set("codigo", produto.getCodigo());
            obj->set("nome", produto.getNome());
            obj->set("categoria", produto.getCategoria());
            obj->set("preco", produto.getPreco());
            obj->set("quantidade_estoque", produto.getQuantidadeEstoque());
            obj->set("estoque_minimo", produto.getEstoqueMinimo());
            obj->set("ativo", produto.isAtivo());
            array->add(obj);
        }
        
        Poco::JSON::Object::Ptr json = new Poco::JSON::Object;
        json->set("produtos", array);
        json->set("total", static_cast<int>(produtos.size()));
        
        ApiServer::enviarResposta(response, json);
    }
}

void ProdutoHandler::handlePost(Poco::Net::HTTPServerRequest& request, 
                               Poco::Net::HTTPServerResponse& response) {
    try {
        std::string body;
        Poco::StreamCopier::copyToString(request.stream(), body);
        
        Poco::JSON::Parser parser;
        auto result = parser.parse(body);
        auto object = result.extract<Poco::JSON::Object::Ptr>();
        
        // Criar produto a partir do JSON
        Produto produto;
        produto.setCodigo(object->getValue<std::string>("codigo"));
        produto.setNome(object->getValue<std::string>("nome"));
        produto.setCategoria(object->optValue<std::string>("categoria", ""));
        produto.setPreco(object->getValue<double>("preco"));
        produto.setQuantidadeEstoque(object->optValue<int>("quantidade_estoque", 0));
        produto.setEstoqueMinimo(object->optValue<int>("estoque_minimo", 10));
        
        if (produto_service->adicionarProduto(produto)) {
            Poco::JSON::Object::Ptr json = new Poco::JSON::Object;
            json->set("id", produto.getId());
            json->set("mensagem", "Produto criado com sucesso");
            
            ApiServer::enviarResposta(response, json, 201);
        } else {
            auto error = ApiServer::criarRespostaErro("Erro ao criar produto", 400);
            ApiServer::enviarResposta(response, error, 400);
        }
    } catch (const std::exception& e) {
        auto error = ApiServer::criarRespostaErro("Dados inválidos: " + std::string(e.what()), 400);
        ApiServer::enviarResposta(response, error, 400);
    }
}

void ProdutoHandler::handlePut(Poco::Net::HTTPServerRequest& request, 
                              Poco::Net::HTTPServerResponse& response) {
    // Implementação da atualização de produto
    auto error = ApiServer::criarRespostaErro("Método PUT não implementado ainda", 501);
    ApiServer::enviarResposta(response, error, 501);
}

void ProdutoHandler::handleDelete(Poco::Net::HTTPServerRequest& request, 
                                 Poco::Net::HTTPServerResponse& response) {
    // Implementação da remoção de produto
    auto error = ApiServer::criarRespostaErro("Método DELETE não implementado ainda", 501);
    ApiServer::enviarResposta(response, error, 501);
}

// VendaHandler Implementation
VendaHandler::VendaHandler(VendaService* service) 
    : venda_service(service) {
}

void VendaHandler::handleRequest(Poco::Net::HTTPServerRequest& request, 
                                Poco::Net::HTTPServerResponse& response) {
    response.setContentType("application/json");
    response.set("Access-Control-Allow-Origin", "*");
    
    try {
        std::string method = request.getMethod();
        
        if (method == "GET") {
            handleGet(request, response);
        } else if (method == "POST") {
            handlePost(request, response);
        } else {
            auto error = ApiServer::criarRespostaErro("Método não suportado", 405);
            ApiServer::enviarResposta(response, error, 405);
        }
    } catch (const std::exception& e) {
        auto error = ApiServer::criarRespostaErro(e.what(), 500);
        ApiServer::enviarResposta(response, error, 500);
    }
}

void VendaHandler::handleGet(Poco::Net::HTTPServerRequest& request, 
                            Poco::Net::HTTPServerResponse& response) {
    auto vendas = venda_service->listarVendas();
    
    Poco::JSON::Array::Ptr array = new Poco::JSON::Array;
    for (const auto& venda : vendas) {
        if (!venda.isCancelada()) {
            Poco::JSON::Object::Ptr obj = new Poco::JSON::Object;
            obj->set("id", venda.getId());
            obj->set("data_venda", venda.getDataVenda().toString().toStdString());
            obj->set("total", venda.getTotal());
            obj->set("forma_pagamento", venda.getFormaPagamentoString());
            obj->set("quantidade_itens", venda.getQuantidadeItens());
            array->add(obj);
        }
    }
    
    Poco::JSON::Object::Ptr json = new Poco::JSON::Object;
    json->set("vendas", array);
    json->set("total", static_cast<int>(array->size()));
    
    ApiServer::enviarResposta(response, json);
}

void VendaHandler::handlePost(Poco::Net::HTTPServerRequest& request, 
                             Poco::Net::HTTPServerResponse& response) {
    auto error = ApiServer::criarRespostaErro("Criação de vendas via API não implementada ainda", 501);
    ApiServer::enviarResposta(response, error, 501);
}

// ApiRequestHandlerFactory Implementation
ApiRequestHandlerFactory::ApiRequestHandlerFactory(ProdutoService* prod_service, 
                                                   VendaService* venda_service)
    : produto_service(prod_service), venda_service(venda_service) {
}

Poco::Net::HTTPRequestHandler* ApiRequestHandlerFactory::createRequestHandler(
    const Poco::Net::HTTPServerRequest& request) {
    
    std::string uri = request.getURI();
    
    if (uri.find("/api/produtos") == 0) {
        return new ProdutoHandler(produto_service);
    } else if (uri.find("/api/vendas") == 0) {
        return new VendaHandler(venda_service);
    }
    
    return nullptr;
}

// ApiServer Implementation
ApiServer::ApiServer(DatabaseManager* db_manager, 
                    ProdutoService* produto_service,
                    VendaService* venda_service,
                    int port)
    : db_manager(db_manager), produto_service(produto_service), 
      venda_service(venda_service), port(port), running(false) {
}

ApiServer::~ApiServer() {
    parar();
}

bool ApiServer::iniciar() {
    try {
        socket = std::make_unique<Poco::Net::ServerSocket>(port);
        
        auto params = new Poco::Net::HTTPServerParams;
        params->setMaxThreads(16);
        params->setMaxQueued(100);
        params->setKeepAlive(true);
        
        auto factory = new ApiRequestHandlerFactory(produto_service, venda_service);
        
        server = std::make_unique<Poco::Net::HTTPServer>(factory, *socket, params);
        server->start();
        
        running = true;
        std::cout << "API Server iniciado na porta " << port << std::endl;
        return true;
        
    } catch (const std::exception& e) {
        std::cerr << "Erro ao iniciar servidor API: " << e.what() << std::endl;
        return false;
    }
}

void ApiServer::parar() {
    if (server && running) {
        server->stop();
        running = false;
        std::cout << "API Server parado" << std::endl;
    }
}

Poco::JSON::Object::Ptr ApiServer::criarRespostaErro(const std::string& mensagem, int codigo) {
    Poco::JSON::Object::Ptr json = new Poco::JSON::Object;
    json->set("error", true);
    json->set("codigo", codigo);
    json->set("mensagem", mensagem);
    return json;
}

Poco::JSON::Object::Ptr ApiServer::criarRespostaSucesso(const std::string& mensagem) {
    Poco::JSON::Object::Ptr json = new Poco::JSON::Object;
    json->set("error", false);
    json->set("mensagem", mensagem);
    return json;
}

void ApiServer::enviarResposta(Poco::Net::HTTPServerResponse& response, 
                              Poco::JSON::Object::Ptr json, 
                              int status) {
    response.setStatus(static_cast<Poco::Net::HTTPResponse::HTTPStatus>(status));
    
    std::ostringstream oss;
    json->stringify(oss);
    
    response.setContentLength(oss.str().length());
    auto& out = response.send();
    out << oss.str();
}
