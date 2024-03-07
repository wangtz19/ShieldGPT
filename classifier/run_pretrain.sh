TRAIN_NUM=5
TEST_NUM=$((10-TRAIN_NUM))

python pre-train.py \
    --batch_size 128 \
    --blr 1e-3 \
    --steps 150000 \
    --mask_ratio 0.9 \
    --data_path "../datasets/cic-ddos2019/image/${TRAIN_NUM}-${TEST_NUM}" \
    --output_dir "../output/cic-ddos2019/${TRAIN_NUM}-${TEST_NUM}/pretrain" \
    --log_dir "../output/cic-ddos2019/${TRAIN_NUM}-${TEST_NUM}/pretrain"