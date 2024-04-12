!# /bin/bash

set -x

mkdir -p ../output/CiCIoT2022/eval

declare -a datasets=("CICIoT2022_MFR" "ISCXTor2016_MFR" "ISCXVPN2016_MFR")
declare -a checkpoints=("checkpoint-step55000.pth" "checkpoint-step10000.pth" "checkpoint-step15000.pth")

length=${#datasets[@]}

for ((i=0;i<$length;i++)); do
    CUDA_VISIBLE_DEVICES=1 python eval.py \
        --resume "/root/YaTC/output/CiCIoT2022/finetune/${datasets[$i]}/${checkpoints[$i]}" \
        --data_path "/root/Vim/dataset/YaTC_datasets/${datasets[$i]}" \
        --output_dir "../output/CiCIoT2022/eval/${datasets[$i]}" \
        --nb_classes 10 \
        --batch_size 256
done