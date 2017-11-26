#include "stringparser.h"
#include <QByteArray>
#include <QDebug>
#include <QRegularExpression>
#include "translation.h"
#include <vector>

void StringParser::setMode(StringParser::TranslationParsingModes flags)
{
    translationMode_ = flags;
}

void StringParser::parseFile(QString path)
{

    // https://regexr.com/3h8qq
    // (?:("|')((?:\\\1|(?:(?!\1).))*)\1)|(?:tr\(("|')((?:\\\3|(?:(?!\3).))*)\3(?:\)|(?:\,[ ]?(?:(\d+)|(?:("|')((?:\\\6|(?:(?!\6).))*)\6)))))
    static QString pattern = QByteArray::fromBase64(
"KD86KCJ8JykoKD86XFxcMXwoPzooPyFcMSkuKSkqKVwxKXwoPzp0clwoKCJ8JykoKD86XFxcM3woPzooPyFcMykuKSkqKVwzKD86XCl8KD86XCxbIF0/KD86KFxkKyl8KD86KCJ8JykoKD86XFxcNnwoPzooPyFcNikuKSkqKVw2KSkpKSk=");
    static QRegularExpression reg(pattern);

    QFile file(path);
    if(!file.open(QIODevice::ReadOnly))
    {
        emit parsingFailed(ParsingError::FileOpen);
        return;
    }

    int countLines = 0;
    while(!file.atEnd()){
        countLines++;
        file.readLine();
    }
    if(!file.seek(0)){
        emit parsingFailed(ParsingError::ReadingFailed); // Return to start failed
        return;
    }

    std::vector<Translation> founds;
    bool isInlineComment = false;
    bool isMultilineComment = false;
    QString line;
    float incProgress = 100 / float(countLines);
    int lineNumber = 1;
#ifdef QT_DEBUG
    qDebug() << "Parse tr : " << translationMode_.testFlag(TranslationParsingMode::ParseTR) << "\n";
    qDebug() << "Parse single : " << translationMode_.testFlag(TranslationParsingMode::ParseSingleQuote) << "\n";
    qDebug() << "Parse double : " << translationMode_.testFlag(TranslationParsingMode::ParseDoubleQuotes) << "\n";
    qDebug() << "Ignore comments : " << translationMode_.testFlag(TranslationParsingMode::IgnoreStringComment) << "\n";
#endif

    while(!file.atEnd())
    {
        line = file.readLine();
        int inLineIndex = line.indexOf("//");

        if(!isMultilineComment)
            isMultilineComment = line.startsWith("/*");
        if(isMultilineComment)
            isMultilineComment = !line.endsWith("*/");

        QRegularExpressionMatchIterator i = reg.globalMatch(line);

#ifdef QT_DEBUG
        qDebug() << "Line " << lineNumber++ << "\n";
#endif
        isInlineComment = false;

        while(i.hasNext())
        {
            QRegularExpressionMatch match = i.next();
            int captLength = match.lastCapturedIndex() + 1;
#ifdef QT_DEBUG
            for(int i = 0; i < captLength; i++)
            {
                QString cap = match.captured(i);
                qDebug() << " CAPTURE AT " << i << " IS " << cap << "\n";
            }
#endif
            Translation t;
            if(captLength == 3) // String only
            {
                if(translationMode_.testFlag(TranslationParsingMode::IgnoreStringComment)){
                    // Check inline for comment
                    if(!isInlineComment && inLineIndex >= 0 && match.capturedStart(0) > inLineIndex) isInlineComment = true;
                    if(isInlineComment || isMultilineComment)
                        continue; // Ignore the string
                }

                if(match.captured(1) == "\"" && !translationMode_.testFlag(TranslationParsingMode::ParseDoubleQuotes))
                    continue; // Ignore double quote strings
                if(match.captured(1) == "'" && !translationMode_.testFlag(TranslationParsingMode::ParseSingleQuote))
                    continue; // Ignore single quote strings
                t.source = match.captured(2);
                if(t.source == '"' || t.source == "'") continue;
            }
            else
            {
                if(!translationMode_.testFlag(TranslationParsingMode::ParseTR)) continue; // Ignore tr

                t.source = match.captured(4);
                if(captLength == 5) // TR with no arg
                {
                    // Nothing to do
                }
                else if(captLength == 6) // TR with id
                {
                    t.contextId = match.captured(5).toInt();
                }
                else if(captLength == 8) // TR with context string
                {
                    t.context = match.captured(7);
                }

            }

#ifdef QT_DEBUG
            qDebug() << "TR = " << t << "\n";
#endif


            if(std::find(founds.begin(), founds.end(), t) == founds.end()) // New translation
            {
                founds.push_back(t);
                emit newTranslation(t);
            }


        }
        emit progress(lineNumber * incProgress);

    }

    file.close();
    emit progress(100);
    emit parsingFinished();
}

void StringParser::parseText(QString)
{
   emit parsingFailed(ParsingError::ParsingNotImplemented);
}
