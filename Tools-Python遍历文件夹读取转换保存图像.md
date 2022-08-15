# Tools-Python遍历文件夹读取转换保存图像

```python
import os
import cv2

Img_dir = "test_img_path"  # 原图地址
Save_dir = "test_save_path" # 转成二进制的地址

def convert(save_dir, shape_file_name, img_read):
    img = img_read
    print(save_dir)
    shape = shape_file_name # (h row, w col, channel)

    # 使用一个元组收取BGR3通道的
    (B, G, R) = cv2.split(img)
    with open (save_dir, 'wb') as fp:
        for i in range(shape[0]): # 先遍历h，后遍历w
            for j in range(shape[1]):
                fp.write(B[i, j])
                fp.write(G[i, j])
                fp.write(R[i, j])

    print("write success!")
    
if __name__ == '__main__':
    count = 1
    for i in os.listdir(Img_dir): # 遍历输入文件夹
        imdir = Img_dir+'/'+i
        img_read = cv2.imread(imdir) # imread读进来就是bgr，但是如果直接用imwrite写则不能写成bgr格式，只能写成.jpeg, .png之类的格式，所以要对每个通道进行操作
        shape_file_name = img_read.shape
        save_dir = Save_dir+'/'+str(count)+'-'+str(shape_file_name[0])+'-'+str(shape_file_name[1])+'-'+str(shape_file_name[2])+'.bgr' # .format(...)也可以
        convert(save_dir, shape_file_name, img_read)
        count+=1
        if count == 6:  # 这里只需要5张即可
            print('convert over')
            break

```

