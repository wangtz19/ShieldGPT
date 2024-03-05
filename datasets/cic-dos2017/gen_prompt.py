import os
import random
# import json
import pandas as pd

random.seed(0)

def capitalize(s):
    return s[0].upper() + s[1:]

NUM_SAMPLE_PER_ATTACK = 5
res_list = []
for attack in ["goldeneye", "hulk", "rudy", "slowbody2", "slowheaders", "slowloris", "slowread"]:
    filenames = list(filter(lambda x: x.endswith(".json"), 
                            os.listdir(f"flow_pcap_5_tuple/test/{attack}")))
    sample_filenames = random.sample(filenames, NUM_SAMPLE_PER_ATTACK)
    for sample_filename in sample_filenames:
        with open(f"flow_pcap_5_tuple/test/{attack}/{sample_filename}", "r") as f:
            text = f.read()
        prompt = f"""
    {text}
    {capitalize(attack)} is one of the DoS attacks. The above is the statistical characteristics of the {attack} attack traffic and some original packet information. 
    First, you are required to analyze the traffic and explain why it is a {attack} attack.
    Then, suppose you are at an ordinary Cisco router, please propose some defense strategies to mitigate this {attack} attack, noting that your advice should be as specific as possible and you are encouraged to provide necessary codes. 
    """
        res_list.append({
            "prompt": prompt,
            "label": attack,
        })
df = pd.DataFrame(res_list)
df.to_csv("attack_prompt_en.csv", index=False)

# with open("attack_prompt.json", "w") as f:
#     json.dump(res, f, indent=2)