import pandas as pd
from tqdm import tqdm
import argparse
from llm import load_model, llm_generate


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument('--model_name', type=str, default='gpt-4',
                        choices=["chatglm3-6b", "Baichuan2-13B-Chat", "Qwen-14B-Chat",
                                 "gpt-3.5-turbo", "gpt-4", "gpt-4-1106-preview",
                                 "Yi-34B-Chat", "Qwen-72B-Chat"])
    args = parser.parse_args()
    model_name = args.model_name
    print(f"Using model {model_name}")
    model, tokenizer = load_model(model_name)

    df = pd.read_csv("../datasets/cic-dos2017/output/attack_prompt_0312_cisco_a=False.csv")
    tqdm.pandas()
    df[f"reply-{model_name}"] = df["prompt"].progress_apply(
        lambda x: llm_generate(x, model_name, model, tokenizer))
    df.to_csv("../datasets/cic-dos2017/output/attack_prompt_0312_cisco_a=False_out.csv", index=False)