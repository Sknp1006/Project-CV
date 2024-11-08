#### GLCM的特征

1. **对比度 (Contrast)**

- 定义：衡量图像中像素灰度值之间的差异程度，反映图像的清晰度和灰度的变化剧烈程度。
- 物理意义：对比度越大，说明图像的纹理差异越明显，局部灰度变化越剧烈。
- 计算公式：

$$
\text{Contrast} = \sum_{i,j} (i - j)^2 \, p(i, j)
$$

2. **熵 (Entropy)**

- 定义：表示图像纹理的复杂度和随机性，反映图像的无序程度。
- 物理意义：熵越高，说明图像中的纹理越复杂，信息量越大；熵低则表示图像纹理较为简单或均匀。
- 计算公式：

$$
\text{Entropy} = - \sum_{i,j} p(i, j) \, \log(p(i, j) + \epsilon)
$$

（其中 $\epsilon$ 是一个小的常数，用于防止对数零问题）

3. **同质性 (Homogeneity)**

- 定义：测量图像纹理的平滑程度，衡量像素对之间灰度值是否接近。
- 物理意义：同质性值高的图像通常具有较为平滑的纹理，而同质性低表示图像中的灰度差异大。
- 计算公式：

$$
\text{Homogeneity} = \sum_{i,j} \frac{p(i, j)}{1 + |i - j|}
$$

4. **相关性 (Correlation)**

- 定义：衡量相邻像素对之间的灰度相关程度，反映像素值之间的线性关系。
- 物理意义：较高的相关性表示纹理方向一致，像素值呈现一定的线性变化；相关性低则表示纹理较为随机。
- 计算公式：

$$
\text{Correlation} = \sum_{i,j} \frac{(i - \mu_i) \cdot (j - \mu_j) \cdot p(i, j)}{\sigma_i \cdot \sigma_j}
$$

$$
\mu_i = \sum_{i} i \sum_{j} p(i, j) \quad , \quad \mu_j = \sum_{j} j \sum_{i} p(i, j)
$$

$$
\sigma_i = \sqrt{\sum_{i} (i - \mu_i)^2 \sum_{j} p(i, j)} \quad , \quad \sigma_j = \sqrt{\sum_{j} (j - \mu_j)^2 \sum_{i} p(i, j)}
$$

$\mu_i$ 是行的均值、$\mu_j$ 是列的均值。

$\sigma_i$ 是行的标准差、$\sigma_j$ 是列的标准差。

5. **能量 (Energy)**

- 定义：表示纹理的重复性和平滑度，是GLCM中所有元素的平方和。有时也被称为角二阶矩 (Angular Second Moment, ASM)
- 物理意义：能量值越高，说明图像中的灰度分布越均匀，结构越规则。
- 计算公式：

$$
\text{Energy} = \sum_{i,j} p(i, j)^2
$$

6. **最大概率 (Maximum Probability)**

- 定义：表示GLCM中出现频率最高的灰度级对的概率。
- 物理意义：最大概率值越高，说明图像中存在显著的、重复出现的灰度值对。
- 计算公式：

$$
\text{Maximum Probability} = \max(p(i, j))
$$

7. **逆方差 (Inverse Difference Moment)**

- 定义：类似于同质性，反映相邻灰度对之间的差异，平滑图像的逆方差值较高。
- 物理意义：值高说明纹理变化较小、图像平滑；值低则表示纹理较粗糙。
- 计算公式：

$$
\text{Inverse Difference Moment} = \sum_{i,j} \frac{p(i, j)}{1 + (i - j)^2}
$$
