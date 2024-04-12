CUDA_VISIBLE_DEVICES=0 python pre-train.py \
    --batch_size 128 \
    --blr 1e-3 \
    --steps 150000 \
    --mask_ratio 0.9 \
    --data_path "/root/Vim/dataset/YaTC_all" \
    --output_dir "../output/CiCIoT2022/pretrain" \
    --log_dir "../output/CiCIoT2022/pretrain" \
    | tee "../output/CiCIoT2022/pretrain/pretrain.log" 2>&1