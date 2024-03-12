#include "highlighter.h"

Highlighter::Highlighter(QTextDocument *parent)
    : QSyntaxHighlighter{parent}
{
    keywordFormat.setForeground(QColorConstants::Svg::blueviolet);
    keywordFormat.setFontWeight(QFont::Bold);
    commentFormat.setForeground(QColorConstants::Svg::darkgray);
    parenFormat.setBackground(QColorConstants::Svg::yellow);
}

void Highlighter::highlightBlock(const QString &text)
{
    for(int i = 0; i < RULECOUNT; i++){
        QRegularExpressionMatchIterator commentmatch = highlightingRules[i].pattern.globalMatch(text);
        while (commentmatch.hasNext()){
            QRegularExpressionMatch currentmatch = commentmatch.next();
            setFormat(currentmatch.capturedStart(), currentmatch.capturedLength(), highlightingRules[i].format);
        }
    }
}

void Highlighter::highlightParen(int symbolIndex)
{
    QTextCursor cursor(document());
    cursor.setPosition(symbolIndex);
    cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor);
    cursor.setCharFormat(parenFormat);

}

void Highlighter::unhighlightParen(int symbolIndex)
{
    QTextCursor cursor(document());
    cursor.setPosition(symbolIndex);
    cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor);
    cursor.setCharFormat(plainFormat);
}
