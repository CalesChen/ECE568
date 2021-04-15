from socket import *
serverName = '127.0.0.1'
serverPort = 8888
ADDRESS = (serverName, serverPort)

def notify_backend(packageID):
    if not packageID:
        if DEBUG:
            print('packageID is null')
        return
    backend_socket = socket(AF_INET,SOCK_STREAM)
    backend_socket.connect(ADDRESS)
    msg = str(packageID)+'\n'
    backend_socket.send(msg.encode('utf-8'))
    response =  backend_socket.recv(256)
    response = response.decode()
    components = response.split(' ')
    if components[0] == 'received' and components[1]==str(packageID):
        return 'Successfully sent and received'
    else:
        if DEBUG:
            print('backend says: '+response)
        return 'Sent and recievd not the same'