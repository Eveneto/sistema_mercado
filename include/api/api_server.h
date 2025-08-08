#ifndef API_SERVER_H
#define API_SERVER_H

#include <Poco/Net/HTTPServer.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPRequestHandlerFactory.h>
#include <Poco/Net/ServerSocket.h>
#include <Poco/JSON/Object.h>
#include <memory>

class DatabaseManager;
class ProdutoService;
class VendaService;

// Handler para endpoints de produtos
class ProdutoHandler : public Poco::Net::HTTPRequestHandler {
private:
    ProdutoService* produto_service;

public:
    explicit ProdutoHandler(ProdutoService* service);
    void handleRequest(Poco::Net::HTTPServerRequest& request, 
                      Poco::Net::HTTPServerResponse& response) override;

private:
    void handleGet(Poco::Net::HTTPServerRequest& request, 
                   Poco::Net::HTTPServerResponse& response);
    void handlePost(Poco::Net::HTTPServerRequest& request, 
                    Poco::Net::HTTPServerResponse& response);
    void handlePut(Poco::Net::HTTPServerRequest& request, 
                   Poco::Net::HTTPServerResponse& response);
    void handleDelete(Poco::Net::HTTPServerRequest& request, 
                      Poco::Net::HTTPServerResponse& response);
};

// Handler para endpoints de vendas
class VendaHandler : public Poco::Net::HTTPRequestHandler {
private:
    VendaService* venda_service;

public:
    explicit VendaHandler(VendaService* service);
    void handleRequest(Poco::Net::HTTPServerRequest& request, 
                      Poco::Net::HTTPServerResponse& response) override;

private:
    void handleGet(Poco::Net::HTTPServerRequest& request, 
                   Poco::Net::HTTPServerResponse& response);
    void handlePost(Poco::Net::HTTPServerRequest& request, 
                    Poco::Net::HTTPServerResponse& response);
};

// Factory para criar handlers
class ApiRequestHandlerFactory : public Poco::Net::HTTPRequestHandlerFactory {
private:
    ProdutoService* produto_service;
    VendaService* venda_service;

public:
    ApiRequestHandlerFactory(ProdutoService* prod_service, VendaService* venda_service);
    
    Poco::Net::HTTPRequestHandler* createRequestHandler(
        const Poco::Net::HTTPServerRequest& request) override;
};

// Servidor principal da API
class ApiServer {
private:
    std::unique_ptr<Poco::Net::HTTPServer> server;
    std::unique_ptr<Poco::Net::ServerSocket> socket;
    DatabaseManager* db_manager;
    ProdutoService* produto_service;
    VendaService* venda_service;
    int port;
    bool running;

public:
    explicit ApiServer(DatabaseManager* db_manager, 
                      ProdutoService* produto_service,
                      VendaService* venda_service,
                      int port = 8080);
    ~ApiServer();

    bool iniciar();
    void parar();
    bool isRodando() const { return running; }
    int getPorta() const { return port; }
    
    // Utilitários
    static Poco::JSON::Object::Ptr criarRespostaErro(const std::string& mensagem, int codigo = 400);
    static Poco::JSON::Object::Ptr criarRespostaSucesso(const std::string& mensagem = "OK");
    static void enviarResposta(Poco::Net::HTTPServerResponse& response, 
                              Poco::JSON::Object::Ptr json, 
                              int status = 200);
};

#endif // API_SERVER_H
