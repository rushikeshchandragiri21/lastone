import java.io.*;
import java.net.*;
import java.util.concurrent.CopyOnWriteArrayList;

public class Server {
    private static final int PORT = 1234;
    private static CopyOnWriteArrayList<ClientHandler> clients = new CopyOnWriteArrayList<>();

    public static void main(String[] args) {
        try (ServerSocket serverSocket = new ServerSocket(PORT)) {
            System.out.println("Server is listening on port " + PORT);
            
            while (true) {
                Socket socket = serverSocket.accept();
                ClientHandler clientThread = new ClientHandler(socket);
                clients.add(clientThread);
                clientThread.start();
            }
        } catch (IOException ex) {
            System.out.println("Server exception: " + ex.getMessage());
            ex.printStackTrace();
        }
    }

    static void broadcast(String message, ClientHandler excludeUser) {
        for (ClientHandler aClient : clients) {
            if (aClient != excludeUser) {
                aClient.sendMessage(message);
            }
        }
    }

    static void removeClient(ClientHandler client) {
        clients.remove(client);
        if (client.getUserName() != null) {
            System.out.println("The client [" + client.getUserName() + "] disconnected");
            broadcast("The client [" + client.getUserName() + "] has left the chat.", null);
        }
    }

    static class ClientHandler extends Thread {
        private Socket socket;
        private PrintWriter writer;
        private String userName;

        public ClientHandler(Socket socket) {
            this.socket = socket;
        }

        public void run() {
            try {
                InputStream input = socket.getInputStream();
                BufferedReader reader = new BufferedReader(new InputStreamReader(input));
                OutputStream output = socket.getOutputStream();
                writer = new PrintWriter(output, true);

                String userName = reader.readLine();
                this.userName = userName;
                String serverMessage = "New user connected: " + userName;
                System.out.println(serverMessage);
                ChatServer.broadcast(serverMessage, this);

                String clientMessage;

                do {
                    clientMessage = reader.readLine();
                    serverMessage = "[" + userName + "]: " + clientMessage;
                    ChatServer.broadcast(serverMessage, this);

                } while (!clientMessage.equals("bye"));

                ChatServer.removeClient(this);
                socket.close();

            } catch (IOException ex) {
                System.out.println("Error in UserThread: " + ex.getMessage());
                ex.printStackTrace();
            }
        }

        void sendMessage(String message) {
            writer.println(message);
        }

        String getUserName() {
            return this.userName;
        }
    }
}