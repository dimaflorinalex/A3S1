using System.Text;
using System.Text.RegularExpressions;

/*
 Analizator lexical (tokenizare + identificare tip token + linia pe care se afla + lungimea lui)
 Implementare in orice limbaj de programare, pentru orice limbaj de programare
*/

// Deadline: 23 octombrie 2024
// Se prezinta la laborator

// Analizor lexical pentru C#, scris in C#
// Dima Florin-Alexandru - Grupa 362

namespace AnalizorLexical
{
    internal class Program
    {
        enum TokenType
        {
            Keyword,
            Identifier,
            Number,
            Operator,
            Punctuation,
            StringLiteral,
            Comment,
            Unknown
        }

        static readonly string[] Keywords = new string[]
        {
            "int", "float", "char", "double", "if", "else", "for", "while", "return", "void", "include"
        };

        static readonly string[] Operators = new string[]
        {
            "+", "-", "*", "/", "=", "++", "--", "==", "!=", "<", ">", "<=", ">=", "&&", "||", "&", "|", "^", "%"
        };

        static readonly char[] Punctuations = new char[]
        {
            '(', ')', '{', '}', '[', ']', ';', ',', '.'
        };

        class Token
        {
            public TokenType Type { get; private set; }
            
            public string Value { get; private set; }

            public int LineNumber { get; private set; }

            public Token(TokenType type, string value, int lineNumber)
            {
                Type = type;
                Value = value;
                LineNumber = lineNumber;
            }

            public override string ToString()
            {
                return $"Type: {Type} - Line: {LineNumber} - Length: {Value.Length} - Value: \"{Value}\"";
            }
        }

        // Method to check if a string is a keyword
        static bool IsKeyword(string value)
        {
            return Keywords.Any(x => x == value);
        }

        // Method to check if a string is an operator
        static bool IsOperator(string value)
        {
            return Operators.Any(x => x == value);
        }

        // Method to check if a char is a punctuation symbol
        static bool IsPunctuation(char value)
        {
            return Punctuations.Any(x => x == value);
        }

        static List<Token> Tokenize(string code)
        {
            List<Token> tokens = new List<Token>();
            StringBuilder currentToken = new StringBuilder();
            bool insideString = false;
            bool insideComment = false;
            int lineNumber = 1;

            for (int i = 0; i < code.Length; i++)
            {
                char c = code[i];

                if (c == '\n' && i - 1 >= 0 && code[i - 1] == '\r')
                {
                    lineNumber++;
                }

                if (insideString)
                {
                    currentToken.Append(c);

                    if (c == '"')
                    {
                        tokens.Add(new Token(TokenType.StringLiteral, currentToken.ToString(), lineNumber));
                        currentToken.Clear();
                        insideString = false;
                    }

                    continue;
                }

                if (insideComment)
                {
                    if (c == '*' && i + 1 < code.Length && code[i + 1] == '/')
                    {
                        currentToken.Append("*/");
                        tokens.Add(new Token(TokenType.Comment, currentToken.ToString(), lineNumber));
                        currentToken.Clear();
                        insideComment = false;
                        i++;
                    }
                    else
                    {
                        currentToken.Append(c);
                    }

                    continue;
                }

                if (char.IsWhiteSpace(c))
                {
                    if (currentToken.Length > 0)
                    {
                        ProcessToken(currentToken.ToString(), tokens, lineNumber);
                        currentToken.Clear();
                    }

                    continue;
                }

                if (c == '"')
                {
                    if (currentToken.Length > 0)
                    {
                        ProcessToken(currentToken.ToString(), tokens, lineNumber);
                        currentToken.Clear();
                    }
                    insideString = true;
                    currentToken.Append(c);

                    continue;
                }

                if (c == '/' && i + 1 < code.Length && code[i + 1] == '*')
                {
                    if (currentToken.Length > 0)
                    {
                        ProcessToken(currentToken.ToString(), tokens, lineNumber);
                        currentToken.Clear();
                    }
                    insideComment = true;
                    currentToken.Append("/*");
                    i++;

                    continue;
                }

                if (IsOperator(c.ToString()))
                {
                    if (currentToken.Length > 0)
                    {
                        ProcessToken(currentToken.ToString(), tokens, lineNumber);
                        currentToken.Clear();
                    }

                    if (i + 1 < code.Length && IsOperator(c.ToString() + code[i + 1].ToString()))
                    {
                        tokens.Add(new Token(TokenType.Operator, c.ToString() + code[i + 1].ToString(), lineNumber));
                        i++;
                    }
                    else
                    {
                        tokens.Add(new Token(TokenType.Operator, c.ToString(), lineNumber));
                    }

                    continue;
                }

                if (IsPunctuation(c))
                {
                    if (currentToken.Length > 0)
                    {
                        ProcessToken(currentToken.ToString(), tokens, lineNumber);
                        currentToken.Clear();
                    }
                    tokens.Add(new Token(TokenType.Punctuation, c.ToString(), lineNumber));

                    continue;
                }

                currentToken.Append(c);
            }

            if (currentToken.Length > 0)
            {
                ProcessToken(currentToken.ToString(), tokens, lineNumber);
            }

            return tokens;
        }

        static void ProcessToken(string value, List<Token> tokens, int lineNumber)
        {
            if (IsKeyword(value))
            {
                tokens.Add(new Token(TokenType.Keyword, value, lineNumber));
            }
            else if (Regex.IsMatch(value, @"^[a-zA-Z_][a-zA-Z0-9_]*$"))
            {
                tokens.Add(new Token(TokenType.Identifier, value, lineNumber));
            }
            else if (Regex.IsMatch(value, @"^\d+(\.\d+)?$"))
            {
                tokens.Add(new Token(TokenType.Number, value, lineNumber));
            }
            else
            {
                tokens.Add(new Token(TokenType.Unknown, value, lineNumber));
            }
        }

        static async Task Main(string[] args)
        {
            Console.Write("Nume fisier: ");
            string? fileName = Console.ReadLine();

            if (string.IsNullOrWhiteSpace(fileName))
            {
                Console.WriteLine("Nume fisier invalid");
                return;
            }

            string filePath = Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "CodeSnippets", fileName);
            if (!File.Exists(filePath))
            {
                Console.WriteLine("Fisierul nu exista");
                return;
            }

            Task<string> fileContentTask = File.ReadAllTextAsync(filePath);

            Console.WriteLine($"Fisierul {fileName} se afla in curs de citire...");

            string fileContent = await fileContentTask;

            Console.WriteLine($"Fisierul {fileName} a fost incarcat in memorie");

            List<Token> tokens = Tokenize(fileContent);

            foreach (var token in tokens)
            {
                Console.WriteLine(token);
            }
        }
    }
}
