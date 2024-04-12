set -x

for dataset in CICIoT2022_MFR ISCXTor2016_MFR ISCXVPN2016_MFR USTC-TFC2016_MFR ; do
    mkdir -p "../output/CiCIoT2022/finetune/${dataset}"
    CUDA_VISIBLE_DEVICES=3 python fine-tune.py \
        --blr 2e-3 \
        --epochs 500 \
        --nb_classes 10 \
        --finetune "/root/YaTC/output/CiCIoT2022/pretrain/checkpoint-step140000.pth" \
        --data_path "/root/Vim/dataset/YaTC_datasets/${dataset}" \
        --output_dir "../output/CiCIoT2022/finetune/${dataset}" \
        --log_dir "../output/CiCIoT2022/finetune/${dataset}" \
        >> "../output/CiCIoT2022/finetune/${dataset}/finetune.log" 2>&1 &
done