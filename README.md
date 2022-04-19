# SOVAC

<h1 align="center">Projeto Elaborado na UC de Sistemas Operativos</h1>

![](sovac.png?raw=true "SOVAC")

# Objetivos 

Este projeto teve como finalidade o desenvolvimento de uma aplicação em C para simular um sistema de pedidos e respostas genérico, com três tipos de participantes diferentes: clientes, proxies e servidores. Esta aplicação, chamada SOVAC, simula o processo de aquisição e distribuição de vacinas, onde os clientes são as instalações de saúde e farmacêuticas (ex., Centro de Saúde de Alvalade), os proxies são os serviços de distribuição (ex., Alliance Healthcare, S.A.), e os servidores são os laboratórios farmacêuticos que produzem e fornecem as vacinas (ex., Johnson & Johnson, Lda). Uma interação entre cliente-servidor, através do proxy, seria, por exemplo, o Centro de Saúde de Alvalade efetuar uma requisição (pedido) de 150 vacinas (ex., COVID-19 Vaccine Janssen) à Alliance Healthcare, S.A.; este último recebe, processa e reencaminha o pedido para o laboratório farmacêutico Johnson & Johnson, Lda que fornece aquela vacina, após verificar que o laboratório o pode satisfazer; a Johnson & Johnson, Lda recebe a requisição da Alliance Healthcare, S.A. e expede-a, enviando as vacinas diretamente para o Centro de Saúde de Alvalade.
