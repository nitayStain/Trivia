import sys, re

try:
    import cssutils
except ImportError:
    import pip

    pip.main(["install", "cssutils"])

DEFUAULT_XAML_HEADER = """
<ResourceDictionary xmlns="http://schemas.microsoft.com/winfx/2006/xaml/presentation"
                    xmlns:x="http://schemas.microsoft.com/winfx/2006/xaml">
        {content}
</ResourceDictionary>
"""


def parse_css(css_file):
    classes = {}

    # Read CSS file
    with open(css_file, "r") as f:
        css_content = f.read()

    sheet = cssutils.parseString(css_content)

    for rule in sheet:
        if rule.type == cssutils.css.CSSRule.STYLE_RULE:
            class_name = rule.selectorText

            properties = {}
            for prop in rule.style:
                properties[prop.name] = prop.value
            classes[class_name] = properties

    return classes


def lr_strip(string: str, left: str, right: str):
    return string.split(right)[0].split(left)[1]


def parse_key_val(arg, class_name, pattern: str):
    key: str = arg[0].split("--")[1]
    val: str = arg[1]

    key = key.capitalize() + class_name
    return pattern.format(key, val)


def main():
    if len(sys.argv) != 3:
        print("[-] Usage: python xmlhelper.py <css input> <xaml output dist>")
        return
    else:
        input_css = sys.argv[1]
        output_xaml_dist = sys.argv[2]

        classes = parse_css(input_css)

        for cls, data in classes.items():
            cls: str
            parsed_cls = lr_strip(cls, ':root[data-theme="', '"]').strip().capitalize()

            file_data = DEFUAULT_XAML_HEADER

            xaml_content = "\n\t".join(
                map(
                    lambda i: parse_key_val(
                        i, parsed_cls, '<Color x:Key="{0}Color">{1}</Color>'
                    ),
                    data.items(),
                )
            )

            xaml_content += "\n"

            xaml_content += "\n\t".join(
                map(
                    lambda i: parse_key_val(
                        i,
                        parsed_cls,
                        '<SolidColorBrush x:Key="{0}Brush" Color="{1}" />',
                    ),
                    data.items(),
                )
            )

            dir = output_xaml_dist.strip("/") + "/" + parsed_cls + "Palette.xaml"

            with open(dir, "w") as f:
                f.write(file_data.format(content=xaml_content))
                print(f"[+] Created: {dir}")


if __name__ == "__main__":
    main()
