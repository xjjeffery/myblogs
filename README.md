# jeffery blog

写博客的初衷是对自己的学习内容做一些简单的总结，市面上也有很多好的博客软件，但是在此之前还没有了解到一个可以支持在所有的博客中索引查找的博客软件系统。机缘巧合之下认识到 [MkDocs 工具](https://www.mkdocs.org/)，可以生成静态文档，再通过 Github Page 部署静态文档，完成自己的博客部署（此工具可以支持在所有的博客中索引查找指定的内容）。

**为什么要使用 MkDocs**:

* 使用 Markdown 作为标记语言，虽没有 Sphinx 的 RestructuredText 强大，但容易上手，且对中文较为友好。
* 丰富的 Markdown 扩展，足以满足日常文档需求。
* 配置文件使用 YAML，相比 Sphinx 使用 Python 源码进行配置相对简单。
* 强大的离线搜索功能，以及基本的中文关键词搜索支持。
* 标题 permalink anchor 支持 Unicode 字符（Sphinx 仅支持 ASCII）。

## 本地构建

MkDocs 工具支持各种插件扩展静态文档，但是这些插件都是基于 Python 语言开发的，因此在安装时尽量做到环境的隔离，避免修改原生的 Python 环境。此处推荐使用 conda 虚拟环境，使用 conda 创建一个服务博客的环境，推荐两个工具 [Mamba](https://mamba.readthedocs.io/en/latest/) 和 [Anaconda](https://www.anaconda.com/)。

- **Mamab**

```bash
mamba create -n mkdocs-env python=3.10
```

- **Anaconda**

```bash
conda create -n mkdocs-env python=3.10
```

环境创建好以后，安装构建博客需要的插件和依赖，通过 `requirements.txt` 实现将所有的插件和依赖自动化安装。

```bash
pip install -r requirements.txt
```

编译整个项目，完成博客的构建，在本地生成一个目录保存所有静态文档，可以通过命令启动 MkDocs 内置的服务器，可以在浏览器输入 `localhost:8000` 中预览文档。

```bash
mkdocs build
mkdocs server
```

也可以直接打开项目中 `site` 目录中的 HTML 文件进行预览。
