TRAIN_NUM=5
TEST_NUM=$((10-TRAIN_NUM))

python fine-tune.py \
    --blr 2e-3 \
    --epochs 100 \
    --nb_classes 8 \
    --finetune "/root/YaTC/output/cic-dos2017/${TRAIN_NUM}-${TEST_NUM}/pretrain/checkpoint-step150000.pth" \
    --data_path "../datasets/cic-dos2017/image/${TRAIN_NUM}-${TEST_NUM}" \
    --output_dir "../output/cic-dos2017/${TRAIN_NUM}-${TEST_NUM}/finetune" \
    --log_dir "../output/cic-dos2017/${TRAIN_NUM}-${TEST_NUM}/finetune"