# Install Configuration

本目录包含项目的安装和打包配置。

## 文件结构

- `CMakeLists.txt` - 安装规则定义，包含 Windows、macOS 和 Linux 平台的安装配置
- `CPackConfig.cmake` - CPack 打包配置，用于生成 MSI（Windows）和 DMG（macOS）安装包
- `cmake/SetUpDSStore.scpt` - macOS DMG 窗口设置的 AppleScript

## 使用方法

这些配置文件会在主 CMakeLists.txt 中自动包含，无需手动调用。

### 快速参考（Windows MSI）

```powershell
# 1. 构建 Release 版本
cd build
cmake --build . --config Release

# 2. 生成 MSI 安装包（如果 WiX 在 PATH 中）
cpack -C Release -G WIX

# 如果 WiX 不在 PATH 中，先添加到 PATH
$env:PATH = "C:\Program Files (x86)\WiX Toolset v3.14\bin;" + $env:PATH
cpack -C Release -G WIX
```

### 生成安装包

#### Windows (MSI)

**前置要求：**
1. 安装 WiX Toolset v3.x（推荐 v3.11.2 或 v3.14）
   - 下载地址：https://github.com/wixtoolset/wix3/releases
   - 安装时确保选择"添加到系统 PATH"
   - 默认安装路径：`C:\Program Files (x86)\WiX Toolset v3.x\bin`

2. 确保已构建 Release 版本：
   ```bash
   cd build
   cmake --build . --config Release
   ```

**生成 MSI 安装包：**

方法 1：使用 CPack 命令（推荐）
```bash
cd build
cpack -C Release -G WIX
```

方法 2：使用 CMake 构建目标
```bash
cd build
cmake --build . --config Release --target package
```

**如果 WiX 不在系统 PATH 中：**
```powershell
# 临时添加到 PATH（仅当前会话有效）
$env:PATH = "C:\Program Files (x86)\WiX Toolset v3.14\bin;" + $env:PATH
cd build
cpack -C Release -G WIX
```

生成的 MSI 文件位于 `build/` 目录，文件名格式：`NutStash-0.1.0-win64.msi`

**同时生成 ZIP 和 MSI：**
```bash
cd build
cpack -C Release -G ZIP
cpack -C Release -G WIX
```

#### macOS (DMG)

**前置要求：**
1. 确保已构建前端：
   ```bash
   npm run build
   ```

2. 确保已构建 Release 版本：
   ```bash
   cd nutstash/build
   cmake --build . --config Release
   ```

**创建 DMG（使用 DragNDrop 生成器）：**

方法 1：使用 CPack 命令（推荐）
```bash
cd nutstash/build
cpack -C Release -G DragNDrop
```

方法 2：使用 CMake 构建目标
```bash
cd nutstash/build
cmake --build . --config Release --target package
```

生成的 DMG 文件位于 `nutstash/build/` 目录，文件名格式：`NutStash-0.1.0-Darwin-<arch>.dmg`（ARM Mac 为 `NutStash-0.1.0-Darwin-arm64.dmg`）

**说明：**
- CPack 会自动将 `dist/` 目录和 `nutstash.app` 一起打包到 DMG 中
- `dist/` 目录会被复制到 app bundle 的 Resources 目录中
- 需要 Xcode 命令行工具才能生成 DMG 文件

## 配置说明

### 安装规则 (CMakeLists.txt)

安装规则会根据平台自动选择：
- **Windows**: 安装可执行文件、DLL、CEF 资源文件和前端构建产物到安装目录
- **macOS**: 安装 app bundle 和前端构建产物到 app bundle 的 Resources 目录
- **Linux**: 安装可执行文件、库文件和前端构建产物到标准 Linux 目录结构

### 打包配置 (CPackConfig.cmake)

- **Windows**: 使用 WIX 生成 MSI 安装包
- **macOS**: 使用 DragNDrop 生成 DMG 磁盘映像
- **Linux**: 支持生成 TGZ、DEB 和 RPM 包

## 注意事项

### Windows MSI 打包
- **WiX Toolset 版本要求**：必须使用 WiX Toolset v3.x（v3.11.2 或 v3.14），CPack 的 WIX 生成器不支持 WiX v6.0
- **路径配置**：如果 WiX 未添加到系统 PATH，需要在运行 cpack 前临时添加到 PATH，或修改 `CPackConfig.cmake` 中的 `CPACK_WIX_ROOT` 设置
- **验证安装**：可以通过运行 `candle -?` 验证 WiX 是否在 PATH 中

### 通用注意事项
- 确保在运行打包之前已经构建了前端（`npm run build`），因为安装规则会包含 `dist/` 目录
- 确保已构建 Release 版本，打包会使用 Release 构建产物
- macOS 上需要 Xcode 命令行工具才能生成 DMG 文件
- 使用 create-dmg 需要先安装：`brew install create-dmg`




