/*
Program Name: Shunting Yard Algorithm 
Date: 2023-11-10
Author: Faizaan Siddiqui
Module Purpose:
The purpose of this program is to demonstrate how Edsger W. Dijkstra's Shunting Yard Algorithm works. There are many sub-parts/topics to this, like the implementation of Stacks and Binary Expression/Search Tree. We also look at concepts like Infix (which is read in from the expressions.txt file), Postfix, and Evaluation Order(PreOrder, InOrder, PostOrder).
*/

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <sstream>
#include <map>
#include <stack>

using namespace std;

string inputFileNameStr = "expressions.txt";                  // Default location in solution folder

class OperatorMapClass 
{

private:
    typedef map<char, int>    OperatorPrecedenceMapType;
    OperatorPrecedenceMapType operatorMapObj;

public:

    OperatorMapClass() 
    {
        operatorMapObj.insert(OperatorPrecedenceMapType::value_type('+', 1));
        operatorMapObj.insert(OperatorPrecedenceMapType::value_type('-', 1));
        //DONE
        operatorMapObj.insert(OperatorPrecedenceMapType::value_type('*', 2));
        operatorMapObj.insert(OperatorPrecedenceMapType::value_type('/', 2));
    }//OperatorMapClass ()

    bool isStackOperatorHigherPrecedence(char operatorChar, char operatorStackChar)
    {
        return((operatorMapObj.count(operatorStackChar)) && (operatorMapObj.at(operatorStackChar) >= operatorMapObj.at(operatorChar)));
    }//isStackOperatorHigherPrecedence()

    bool  isOperator(char token) 
    {
        //DONE
        return (operatorMapObj.count(token) > 0);
    }//isOperator()

};//OperatorMapClass

OperatorMapClass  operatorMapObj;

class ShuntingYardClass 
{

public:

    string createPostFixFrom(string infixString) 
    {
        string       outputString;
        stack <char> operatorStackObj;

        for (char token : infixString) 
        {
            switch (token) {
            case '/': case '*': case '+': case '-':
                //DONE
                while (!operatorStackObj.empty()&&operatorStackObj.top()!='('&& operatorMapObj.isStackOperatorHigherPrecedence(token,operatorStackObj.top()))
                {
                    outputString += operatorStackObj.top();
                    operatorStackObj.pop();
                }
                operatorStackObj.push(token);
                break;
            case '(':                   
                //DONE
                operatorStackObj.push(token);
                break;
            case ')': 
                //DONE
                while (!operatorStackObj.empty() && operatorStackObj.top() != '(')
                {
                    outputString += operatorStackObj.top();
                    operatorStackObj.pop();
                }
                operatorStackObj.pop();
                break;
            default:                                                                                                             
                //DONE
                outputString += token;
                break;
            }
        }
        //DONE
        while (!operatorStackObj.empty())
        {
            outputString += operatorStackObj.top();
            operatorStackObj.pop();
        }
        return(outputString);

    }//postfix()	

};//ShuntingYardClass



class TreeNodeClass 
{

public:
    TreeNodeClass* left;
    char            value;
    TreeNodeClass* right;

};//TreeNodeClass

TreeNodeClass* BuildNewNodeObjPtrMethod(char value) 
{
    //DONE
    TreeNodeClass* newNode = new TreeNodeClass();
    newNode->value = value;
    newNode->left = newNode->right = nullptr;
    return newNode;
};

TreeNodeClass* ConstructBET(string postFixStr)
{
    //DONE
    stack<TreeNodeClass*> parseStack;

    for (char token : postFixStr)
    {
        TreeNodeClass* newNodePtr = BuildNewNodeObjPtrMethod(token);

        if (operatorMapObj.isOperator(token)) 
        {
            newNodePtr->right = parseStack.top();
            parseStack.pop();

            newNodePtr->left = parseStack.top();
            parseStack.pop();
        }

        parseStack.push(newNodePtr);
    }

    return parseStack.top();
}


string buildString;

void preorder(TreeNodeClass* treeNode) 
{
    //DONE
    if (treeNode == nullptr)
        return;
    buildString += treeNode->value;
    preorder(treeNode->left);
    preorder(treeNode->right);
}

bool areParensRequired(TreeNodeClass* treeNode, char value) 
{
    OperatorMapClass operatorMapObj;
    if (operatorMapObj.isOperator(value) &&
        operatorMapObj.isOperator(treeNode->value) &&
        operatorMapObj.isStackOperatorHigherPrecedence(treeNode->value, value)) {
        buildString += '(';
        return true;
    }
    return false;
}


void inorder(TreeNodeClass* treeNode) 
{
    //DONE
    bool parensRequired = false;
    if (treeNode) 
    {
        parensRequired = areParensRequired(treeNode->left, treeNode->value);
        inorder(treeNode->left);
        if (parensRequired)
            buildString += ')';

        buildString += treeNode->value;

        parensRequired = areParensRequired(treeNode->right, treeNode->value);
        inorder(treeNode->right);
        if (parensRequired)
            buildString += ')';

    }
}


void postorder(TreeNodeClass* treeNode) 
{
    //DONE
    if (treeNode == nullptr)
        return;
    postorder(treeNode->left);
    postorder(treeNode->right);
    buildString += treeNode->value;

}



int main() 
{

    ifstream  inputFileStreamObj(inputFileNameStr);

    if (inputFileStreamObj.fail()) {
        cout << "File could not be opened !" << endl;

        return (EXIT_FAILURE);
    }//if

    string  infixExpressionStr,
        postfixExpressionStr;

    ShuntingYardClass shuntingYardObj;

    while (inputFileStreamObj >> infixExpressionStr) 
    {

        cout << "InFix   Expression : " << infixExpressionStr << endl;
        postfixExpressionStr = shuntingYardObj.createPostFixFrom(infixExpressionStr);
        cout << "PostFix Expression : " << postfixExpressionStr << endl << endl;

        TreeNodeClass* expressionTreeRootPtr = ConstructBET(postfixExpressionStr);

        buildString = "";  preorder(expressionTreeRootPtr);
        cout << "Tree  pre-order expression is " << endl << buildString << endl << endl;

        buildString = "";  inorder(expressionTreeRootPtr);
        cout << "Tree   in-order expression is " << endl << buildString << endl << endl;

        buildString = "";  postorder(expressionTreeRootPtr);
        cout << "Tree post-order expression is " << endl << buildString << endl << endl;

        cout << endl << endl;

    };//while

    inputFileStreamObj.close();

    cout << "Press the enter key once or twice to end." << endl;

    return EXIT_SUCCESS;

}

