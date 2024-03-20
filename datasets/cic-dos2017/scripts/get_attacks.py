def parse_line(line):
    attack, others = line.strip("\n").split("to")
    attack = attack.strip()
    # print(f"attack: {attack}")
    dst_ip, others = others.split("after")
    dst_ip = dst_ip.strip()
    # print(f"dst_ip: {dst_ip}")
    offset = others.strip(" \n").split(" ")[0]
    # print(f"offset: {offset}")
    assert ":" in offset
    h, m = offset.split(":")
    h, m = int(h), int(m)
    return attack, dst_ip, 60*h + m

# print(parse_line("slowread	 to		75.127.97.72	 after 01:58 minutes"))
# print(parse_line("slowloris	 to		75.127.97.72	 after 16:33 993 minutes"))

attacks_dict = {}
with open("attacks.txt", "r") as f:
    lines = f.readlines()
    for idx, line in enumerate(lines):
        if len(line) < 2:
            continue
        attack, dst_ip, start_offset = parse_line(line)
        if idx < len(lines) - 1:
            next_line = lines[idx + 1]
            if len(next_line) > 5:
                _, _, end_offset = parse_line(next_line)
            else:
                end_offset = None
        else:
            end_offset = None
        if attack not in attacks_dict:
            attacks_dict[attack] = [{
                "dst_ip": dst_ip,
                "start_offset": start_offset,
                "end_offset": end_offset,
            }]
        else:
            attacks_dict[attack].append({
                "dst_ip": dst_ip,
                "start_offset": start_offset,
                "end_offset": end_offset,
            })
import json
with open("attacks_dict.json", "w") as f:
    json.dump(attacks_dict, f, indent=2)