python fine-tune.py \
    --blr 2e-3 \
    --epochs 100 \
    --nb_classes 8 \
    --finetune /root/YaTC/output/cic-ddos2019/8-2/pretrain/checkpoint-step150000.pth \
    --data_path ../datasets/cic-ddos2019/image/8-2 \
    --output_dir ../output/cic-ddos2019/finetune \
    --log_dir ../output/cic-ddos2019/finetune