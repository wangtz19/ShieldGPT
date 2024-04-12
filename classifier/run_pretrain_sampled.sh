mkdir -p "../output/pretrain-sampled"

CUDA_VISIBLE_DEVICES=2 python pre-train.py \
    --batch_size 128 \
    --blr 1e-3 \
    --steps 150000 \
    --mask_ratio 0.9 \
    --data_path "/root/Vim/dataset/Pretrain_sampled" \
    --output_dir "../output/pretrain-sampled" \
    --log_dir "../output/pretrain-sampled" \
    | tee "../output/pretrain-sampled/pretrain.log" 2>&1