for model_name in "gpt-4" #"gpt-4-1106-preview" #"gpt-4" "chatglm3-6b" "Qwen-14B-Chat" "Yi-34B-Chat" "Qwen-72B-Chat"
do
    echo "Running $model_name"
    python gen_test.py --model_name $model_name
done