from socket import *
from .models import *
import math
serverName = 'vcm-19383.vm.duke.edu'
#'127.0.0.1'
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
        print('backend says: '+response)
        return 'Sent and recievd not the same'

def get_closest_wh(address_x, address_y):
    min_dist = float('inf')
    whID = 1
    all_whs = Warehouse.objects.all()
    for wh in all_whs:
        wh_x = wh.address_x
        wh_y = wh.address_y
        dist = math.sqrt(math.pow(address_x-wh_x,2)+math.pow(address_y-wh_y,2))
        if dist<min_dist:
            min_dist = dist
            whID = wh.id
    return whID