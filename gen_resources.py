"""生成游戏所需的占位资源图片（优化版）"""
import struct
import zlib
import os

def create_png(width, height, r, g, b):
    """用更高效的方式生成纯色 PNG"""
    # 一行数据：filter byte + width个像素
    row = b'\x00' + bytes([r, g, b]) * width
    raw = row * height  # 直接重复整行，比逐像素循环快得多
    compressed = zlib.compress(raw)

    sig = b'\x89PNG\r\n\x1a\n'
    def chunk(t, d):
        return struct.pack('>I', len(d)) + t + d + struct.pack('>I', zlib.crc32(t + d) & 0xffffffff)
    ihdr = chunk(b'IHDR', struct.pack('>IIBBBBB', width, height, 8, 2, 0, 0, 0))
    idat = chunk(b'IDAT', compressed)
    iend = chunk(b'IEND', b'')
    return sig + ihdr + idat + iend

def create_btn_png(width, height, r, g, b, br, bg, bb):
    """带边框的按钮 - 用 row 拼接"""
    def make_row(is_top_or_bottom):
        if is_top_or_bottom:
            return b'\x00' + bytes([br, bg, bb]) * width
        # 中间行：边框+填充+边框
        return b'\x00' + bytes([br, bg, bb]) * 2 + bytes([r, g, b]) * (width - 4) + bytes([br, bg, bb]) * 2

    rows = []
    rows.append(make_row(True))   # top
    rows.append(make_row(True))   # top-1
    for _ in range(height - 4):   # middle
        rows.append(make_row(False))
    rows.append(make_row(True))   # bottom-1
    rows.append(make_row(True))   # bottom
    raw = b''.join(rows)
    compressed = zlib.compress(raw)

    sig = b'\x89PNG\r\n\x1a\n'
    def chunk(t, d):
        return struct.pack('>I', len(d)) + t + d + struct.pack('>I', zlib.crc32(t + d) & 0xffffffff)
    ihdr = chunk(b'IHDR', struct.pack('>IIBBBBB', width, height, 8, 2, 0, 0, 0))
    idat = chunk(b'IDAT', compressed)
    iend = chunk(b'IEND', b'')
    return sig + ihdr + idat + iend

def main():
    res_dir = os.path.join(os.path.dirname(os.path.abspath(__file__)), 'Resources')
    os.makedirs(res_dir, exist_ok=True)

    resources = {
        'ground.png':            create_png(2400, 1800, 50, 60, 40),
        'player.png':            create_png(32, 48, 100, 180, 255),
        'monster.png':           create_png(28, 28, 255, 80, 80),
        'building.png':          create_png(80, 80, 120, 100, 80),
        'npc.png':               create_png(24, 32, 200, 200, 100),
        'btn_normal.png':        create_btn_png(160, 50, 60, 70, 90, 100, 110, 130),
        'btn_pressed.png':       create_btn_png(160, 50, 80, 90, 110, 100, 110, 130),
        'btn_small.png':         create_btn_png(120, 36, 60, 70, 90, 100, 110, 130),
        'btn_small_pressed.png': create_btn_png(120, 36, 80, 90, 110, 100, 110, 130),
        'rect.png':              create_png(320, 44, 40, 45, 60),
        'panel_bg.png':          create_png(480, 400, 30, 35, 50),
        'attack_btn.png':        create_png(80, 80, 200, 60, 60),
        'attack_btn_pressed.png':create_png(80, 80, 180, 80, 80),
    }

    for name, data in resources.items():
        path = os.path.join(res_dir, name)
        with open(path, 'wb') as f:
            f.write(data)
        print(f'  [OK] {name} ({len(data)} bytes)')

    print(f'\n所有资源已生成到: {res_dir}')

if __name__ == '__main__':
    main()
