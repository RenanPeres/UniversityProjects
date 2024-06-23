import ktrain
from sklearn import metrics
import matplotlib.pyplot as plt
import pandas as pd
from sklearn.metrics import classification_report

NAME_TERMINAL = "ep3_context"


#Importando a biblioteca pandas e carregando as amostras
df_test = pd.read_pickle('Amostras_Teste.pkl')
print(df_test.shape)
df_test = df_test.dropna()
print(df_test.shape)
print("importou as bibliotecas")

#Load predictor
p = ktrain.load_predictor('bert-base-multilingual-cased_e3_apps.model')
print("carregou o modelo")

# teste com contexto
x_test = df_test[['review', 'category']].values
x_test = list(map(tuple, x_test))
y_true = [df_test.label_feelings.values, df_test.negative_index.values]

#Amostras gerais
print(NAME_TERMINAL)

#Predições
y_pred = p.predict(x_test)
print("terminado as predições")
y_pred_proba = p.predict_proba(x_test)
print("terminado as probabilidades de predições")

#Exibindo as métricas obtidas
print(classification_report(y_true[0][0:len(y_pred)], y_pred))

#Calculando a curva ROC e AUC
fpr, tpr, _ = metrics.roc_curve(y_true[1],  y_pred_proba[:, 0])
auc = metrics.roc_auc_score(y_true[1],  y_pred_proba[:, 0])

#create ROC curve
plt.figure()
lw = 2
plt.plot(
    fpr,
    tpr,
    color="darkorange",
    lw=lw,
    label="ROC curve (area = %0.4f)" % auc,
)
plt.xlabel("False Positive Rate")
plt.ylabel("True Positive Rate")
plt.legend(loc="lower right")
plt.savefig("./00 - imagens/"+NAME_TERMINAL+".png", 
            dpi=300,
            transparent=True)

print("Fim da execução de "+NAME_TERMINAL)