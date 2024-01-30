#include "highlighter.h"

Highlighter::Highlighter(QTextDocument *parent)
    : QSyntaxHighlighter{parent}
{
    dumbFormat.setForeground(QColorConstants::Svg::orange);
    dumbFormat.setBackground(Qt::green);
    dumbFormat.setFontWeight(QFont::Bold);
}

void Highlighter::highlightBlock(const QString &text)
{
    setFormat(0, text.length(), dumbFormat);
}
