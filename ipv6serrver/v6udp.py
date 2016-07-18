import threading
import socket
import time
import datetime

def fetch_local_ipv6_address(port=10008):
    # try to detect whether IPv6 is supported at the present system and
    # fetch the IPv6 address of localhost.
    if not socket.has_ipv6:
        raise Exception("the local machine has no IPv6 support enabled")
 
    addrs = socket.getaddrinfo("localhost", port, socket.AF_INET6, 0, socket.SOL_UDP)
    # example output: [(23, 0, 6, '', ('::1', 10008, 0, 0))]
 
    if len(addrs) == 0:
        raise Exception("there is no IPv6 address configured for localhost")
 
    entry0 = addrs[0]
    sockaddr = entry0[-1]
    return sockaddr
 
 
def ipv6_echo_server(sockaddr):
    # Echo server program
 
    s = socket.socket(socket.AF_INET6, socket.SOCK_DGRAM)
    s.bind(sockaddr)
    while True:
        data, addr = s.recvfrom(1024) # buffer size is 1024 bytes
        print ("received message:", data)
        if data is not "":
            s1 = socket.socket(socket.AF_INET6, socket.SOCK_DGRAM)
            s1.connect(addr)
            s1.send(data)
            s1.close()
 
 
def ipv6_echo_client(sockaddr):
    # Echo client program
    # use hostname or port number or use 'sockaddr' to open the connection
 
    #HOST = 'localhost'
    #PORT = 10008              # The same port as used by the server
    s = socket.socket(socket.AF_INET6, socket.SOCK_DGRAM,0)
    #s.connect((HOST, PORT))
    s.connect(sockaddr)
 
    print ("client opened socket connection:", s.getsockname())
    data = 'Hello, world! -> via IPv6 :-) at time:' + str(datetime.datetime.now())
    print ('Client is sending:', repr(data))
 
    s.send(data.encode())
   # data = s.recv(1024).decode()
    s.close()
    print ('Client received response:', repr(data))
 
if __name__ == "__main__":
    try:
        # fetch the local IPv6 address
        local_ipv6_addr = fetch_local_ipv6_address()
        print(local_ipv6_addr)
        t = threading.Thread(target=ipv6_echo_server, args=(local_ipv6_addr,))
        t.start()
        xx = 0
        while xx==0:
            time.sleep(1)
            ipv6_echo_client(local_ipv6_addr)
             
    except Exception as e:
        print ("Error occurred: ", e)
	 
    print ("bye.")

def holdserverUdp(port = 10008):
	local_ipv6_addr = fetch_local_ipv6_address(port)
	print(local_ipv6_addr)
	t = threading.Thread(target=ipv6_echo_server, args=(local_ipv6_addr,))
	t.start()
