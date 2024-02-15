#ifndef HIGHLIGHTER_H
#define HIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QRegularExpression>
#include <QTextEdit>

#define RULECOUNT 8

class Highlighter : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    Highlighter(QTextDocument *parent = nullptr);
    void highlightParen(int symbolIndex);
    void unhighlightParen(int symbolIndex);

protected:
    void highlightBlock(const QString &text) override;

private:
    struct HighlightingRule
    {
        QRegularExpression pattern;
        const QTextCharFormat& format;
    };
    QTextCharFormat keywordFormat;
    QTextCharFormat commentFormat;
    QTextCharFormat parenFormat;
    QTextCharFormat plainFormat;
    HighlightingRule highlightingRules [RULECOUNT] = {
        {QRegularExpression("\\bdefine\\b"), keywordFormat},
        {QRegularExpression("\\bset!\\s"), keywordFormat},
        {QRegularExpression("\\blet\\b"), keywordFormat},
        {QRegularExpression("\\bcond\\b"), keywordFormat},
        {QRegularExpression("\\bcons\\b"), keywordFormat},
        {QRegularExpression("\\bcar\\b"), keywordFormat},
        {QRegularExpression("\\bcdr\\b"), keywordFormat},
        {QRegularExpression(";.*"), commentFormat}
    };
};

#endif // HIGHLIGHTER_H
