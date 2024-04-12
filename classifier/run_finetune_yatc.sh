python fine-tune.py \
    --blr 2e-3 \
    --epochs 500 \
    --nb_classes 10 \
    --finetune "/root/YaTC/output/CiCIoT2022/pretrain/checkpoint-step140000.pth" \
    --data_path "/root/Vim/dataset/YaTC_datasets/CICIoT2022_MFR" \
    --output_dir "../output/CiCIoT2022/finetune/yatc" \
    --log_dir "../output/CiCIoT2022/finetune/yatc"