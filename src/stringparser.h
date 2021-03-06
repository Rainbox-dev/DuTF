#ifndef STRINGPARSER_H
#define STRINGPARSER_H

#include "parser.h"
#include "translation.h"

#include <QRegularExpressionMatch>
#include <vector>

/**
 * @brief Use this parser to extract string and tr calls from a file
 */
class StringParser : public Parser
{
    Q_OBJECT

public:

    enum TranslationParsingMode {
        ParseTR = 0x0001,
        ParseSingleQuote = 0x0010,
        ParseDoubleQuotes = 0x0100,
        IgnoreStringComment = 0x1000,
        Export = 0x10000
    };
    Q_DECLARE_FLAGS(TranslationParsingModes, TranslationParsingMode)

    /**
     * @brief Set the file path to which the parser will export the new source code
     */
    void setExportFileName(const QString&);

    /**
     * @brief Set the mode that will be used when parsing the file
     */
    void setMode(StringParser::TranslationParsingModes);

    /**
     * @brief Give a link to the current wip translations.
     * Used for updating user source code
     */
    void setTranslations(std::vector<Translation>*);

signals:

    /**
     * @brief Emitted when a new translation has been parsed
     */
    void newTranslation(Translation);

protected:

    void parseFile(QString);

    void parseText(QString);

    StringParser::TranslationParsingModes translationMode_;

    /**
     * @brief Process the text matched by the regex
     *
     * The method wil take care of checking if the translation is already present or not
     *
     * @param   The regex match
     * @param   The resulting list of translations
     */
    void processImportMatch(QRegularExpressionMatch, std::vector<Translation>&);

    /**
     * @brief Process the current line while exporting
     *
     * Depending on the flags, add tr or not on the line
     *
     * @param   The line
     * @param   The output file
     */
    void processExportLine(QString, QFile&);

    /**
     * @brief Translations that the user is currently working on
     */
    std::vector<Translation> * translations;

    /**
     * @brief The filename of the exported file if any
     */
    QString exportFileName;

};

Q_DECLARE_OPERATORS_FOR_FLAGS(StringParser::TranslationParsingModes)
Q_DECLARE_METATYPE(StringParser::TranslationParsingModes)

#endif // STRINGPARSER_H
