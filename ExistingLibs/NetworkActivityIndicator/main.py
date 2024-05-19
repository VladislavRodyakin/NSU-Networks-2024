#https://readthedocs.org/projects/scapy/downloads/pdf/stable/
from scapy.all import sniff
from matplotlib import pyplot as plt, animation as anim
from datetime import datetime

print("Basic filter options:")
print("    tcp: Capture only TCP packets.")
print("    udp: Capture only UDP packets.")
print("    [src, dst] host <ip_address>: Capture packets with the specified IP address as source or destination. src or dst prefixes are used to specify the ip_address purpose.")
print("    gateway <ip_address>: Capture packets with the specified IP address as the gateway.")
print("Generally program supports Berkeley Packet Filter (BPF) syntax. It can be found at: https://biot.com/capstats/bpf.html")

user_filter = input("Enter filter expression. Empty input will disable filtering): ").strip()

capture_times = [0]
packet_sizes = [0]

SNIFFING_TIME = 0.1 # seconds
GRAPH_UPDATE_TIME = SNIFFING_TIME*1000 # milliseconds
TIME_WINDOW = 15 # secondss

figure = plt.figure(figsize=(10, 7))
graph = plt.subplot(1,1,1)
start = datetime.now()

def handle_packet(receved_packet):
    packet_sizes[-1] += len(receved_packet)


def update_plot(frame):
    
    sniff(prn=handle_packet, filter=user_filter, timeout=SNIFFING_TIME)

    if (datetime.now()-start).total_seconds() > TIME_WINDOW:
        packet_sizes.pop(0)
        capture_times.pop(0)

    graph.clear()
    graph.plot(capture_times, packet_sizes)

    packet_sizes.append(0)
    capture_times.append((datetime.now()-start).total_seconds())
    

plt.title('Network activity')
plt.xlabel('Seconds, s')
plt.ylabel('Data, bytes')
animation = anim.FuncAnimation(figure, update_plot, interval=GRAPH_UPDATE_TIME)
plt.show()
