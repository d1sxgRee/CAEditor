#ifndef HIGHLIGHTER_H
#define HIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QRegularExpression>

#define RULECOUNT 8

class Highlighter : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    Highlighter(QTextDocument *parent = nullptr);

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
