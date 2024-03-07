import os

# 1. categorize the pcap files into two folders: first_day and second_day
# os.makedirs("../pcap/first_day", exist_ok=True)
# os.makedirs("../pcap/second_day", exist_ok=True)

# first_day_filenames = list(filter(lambda x: x.startswith("SAT-03-11") and os.path.isfile("../pcap/" + x), 
#                                 os.listdir("../pcap")))
# second_day_filenames = list(filter(lambda x: x.startswith("SAT-01-12") and os.path.isfile("../pcap/" + x),
#                                 os.listdir("../pcap")))
# for filename in first_day_filenames:
#     os.rename(f"../pcap/{filename}", f"../pcap/first_day/{filename}.pcap")
# for filename in second_day_filenames:
#     os.rename(f"../pcap/{filename}", f"../pcap/second_day/{filename}.pcap")
# print("Done")

# 2. merge all pcap files in the first_day into one file
os.system("mergecap -w ../pcap/first_day.pcap ../pcap/first_day/*.pcap")