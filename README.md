# Project-CV
A reset version of the [Project-AWCV](https://github.com/Sknp1006/Project-AWCV) repository.

本项目旨在：
1. 练习 C++ Opencv 的使用
2. 练习 cmake 的使用
3. 练习 google test 的使用
4. 优化代码结构和质量
5. 练习规范提交
6. ...

## 目录结构
- `3rdparty/` :存放第三方库
- `src` :存放源代码
- `test` :存放测试代码

## 项目依赖

除了 Opencv 之外，其他依赖库已经放在 `3rdparty/` 目录下。


如果不会编译 Opencv，可以参考 [【交叉编译】opencv4](https://sknp.top/posts/cross-compile-opencv)。

将其中的编译器替换为自己的编译器即可。

## 一键编译

在准备好 opencv 环境之后，可以直接运行 `build.sh` 脚本进行编译。

```bash
./build.sh
```

## Commit 规范
> [https://fe-notes.yunyoujun.cn/common/dev/#commit-message](https://fe-notes.yunyoujun.cn/common/dev/#commit-message)

- **build** ：对构建系统或者外部依赖项进行了修改
- **ci** ：对CI配置文件或脚本进行了修改
- **docs** ：对文档进行了修改
- **feat** ：增加新的特征
- **fix** ：修复bug
- **pref** ：提高性能的代码更改
- **refactor** ：既不是修复bug也不是添加特征的代码重构
- **style** ：不影响代码含义的修改，比如空格、格式化、缺失的分号等
- **test** ：增加确实的测试或者矫正已存在的测试
