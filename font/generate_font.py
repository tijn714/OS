def read_psf1_font(filename):
    with open(filename, 'rb') as f:
        header = f.read(4)
        if header[0] != 0x36 or header[1] != 0x04:
            raise ValueError("Not a valid PSF1 font file")
        mode = header[2]
        charsize = header[3]
        font_data = f.read()
        return font_data, charsize

def generate_c_array(font_data, charsize):
    c_array = []
    for i in range(0, len(font_data), charsize):
        glyph = font_data[i:i+charsize]
        c_array.append(glyph)
    return c_array

def write_font_header(c_array, charsize, output_file):
    with open(output_file, 'w') as f:
        f.write('#ifndef FONT_H\n')
        f.write('#define FONT_H\n\n')
        f.write(f'const unsigned char font[{len(c_array)}][{charsize}] = {{\n')
        
        for glyph in c_array:
            f.write('    { ')
            f.write(', '.join(f'0x{byte:02x}' for byte in glyph))
            f.write(' },\n')
        
        f.write('};\n\n')
        f.write('#endif // FONT_H\n')

# Main script
font_data, charsize = read_psf1_font('vga16.psf')
c_array = generate_c_array(font_data, charsize)
write_font_header(c_array, charsize, 'font.h')
