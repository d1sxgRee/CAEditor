#include "highlighter.h"

Highlighter::Highlighter(QTextDocument *parent)
    : QSyntaxHighlighter{parent}
{
    keywordFormat.setForeground(QColorConstants::Svg::blueviolet);
    keywordFormat.setFontWeight(QFont::Bold);
    commentFormat.setForeground(QColorConstants::Svg::darkgray);

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
