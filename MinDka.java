import java.util.*;
import java.util.stream.Collectors;


public class MinDka {

    public static void main(String[] args){

        Scanner sc = new Scanner(System.in);

        //System.out.println("ggrgr");

        //1.redak
        String stanjaString = sc.nextLine();
        List<String> listaStanja2 = List.of(stanjaString.split(","));
        List<String> listaStanja = listaStanja2.stream().collect(Collectors.toList());

        //2.redak
        String simboliString = sc.nextLine();
        List<String> simboli2 = List.of(simboliString.split(","));
        List<String> simboli = simboli2.stream().collect(Collectors.toList());

        //3.redak
        String prihvStanjaString = sc.nextLine();
        List<String> prihvStanja2 = List.of(prihvStanjaString.split(","));
        List<String> prihvStanja = prihvStanja2.stream().collect(Collectors.toList());

        //4.redak
        String pocetnoStanje = sc.nextLine();

        //5. i ostali redovi
        List<Vector<String>> prijelazi = new LinkedList<>();
        String funkc;
        while (sc.hasNextLine()){
            funkc = sc.nextLine();
            if (funkc.equals("")){
                break;
            }
            Vector<String> vector = new Vector<>();
            String[] pom1 = funkc.split("->");
            String[] pom2 = pom1[0].split(",");
            vector.add(pom2[0]);
            vector.add(pom2[1]);
            vector.add(pom1[1]);
            //System.out.println(vector.toString());

            prijelazi.add(vector);

        }

        //trazenje dohvatljivih stanja
        Set<String> dohvatljivaStanja = new TreeSet<>();
        Stack<String> stog = new Stack<>();
        stog.add(pocetnoStanje);

        while (!stog.isEmpty()){
            String trenutnoStanje = stog.pop();
            dohvatljivaStanja.add(trenutnoStanje);

            for (Vector<String> vc : prijelazi){
                if (vc.get(0).equals(trenutnoStanje) && !dohvatljivaStanja.contains(vc.get(2))){
                    stog.push(vc.get(2));
                }
            }

        }

        //izbacivanje nedohvatljivih stanja
        List<String> pom = new ArrayList<>();
        for (String s : listaStanja){
            if (!dohvatljivaStanja.contains(s)){
                pom.add(s);
            }
        }

        //micanje funkcija sa nedostiznim stanjima
        List<Vector<String>> vcMicanje = new ArrayList<>();
        for (String s : pom){
            for (Vector<String> vector : prijelazi){
                if(vector.get(0).equals(s)){
                    vcMicanje.add(vector);
                }
            }
        }
        prijelazi.removeAll(vcMicanje);
        listaStanja.removeAll(pom);

        //System.out.println("printanje nakon stog akcije");
        //System.out.println(listaStanja);

        //pocetna podjela na prihvatljiva i neprihvatljiva stanja
        List<List<String>> lista_listi = new ArrayList<>();

        List<String> prihvStanjaPom = new ArrayList<>();
        for (String s : prihvStanja){
            if (listaStanja.contains(s)){
                prihvStanjaPom.add(s);
            }
        }


        if (!prihvStanjaPom.isEmpty()){
            lista_listi.add(prihvStanjaPom);
        }


        List<String> neprihvStanja = new ArrayList<>();
        for (String s : listaStanja){
            if (!prihvStanja.contains(s)){
                neprihvStanja.add(s);
            }
        }

        if (!neprihvStanja.isEmpty()){
            lista_listi.add(neprihvStanja);
        }

        //System.out.println("printam prihv stanja 1");
        //System.out.println(prihvStanja);
        //System.out.println(neprihvStanja);

        //algoritam 2
        boolean provjera = true;

        while (provjera){

            provjera = false;

            //kopija liste za iteraciju;
            List<List<String>> lista_listi_pom = new ArrayList<>();
            for(List<String> lista : lista_listi){
                List<String> kopija = new ArrayList<>(lista);
                lista_listi_pom.add(kopija);
            }


            //System.out.println(lista_listi);

            //petlja po skupovima
            for (List<String> trenutniSkup : lista_listi_pom){

                if (trenutniSkup.size() == 1){
                    continue;
                }

                //lista za odvojit od trenutnog skupa
                List<String> novaLista = new ArrayList<>();


                //sortiranje prije ulaska
                Collections.sort(trenutniSkup);
                //petlja za prvi clan para
                for (int i = 0; i < trenutniSkup.size() - 1; i++){

                    //petlja za drugi clan para
                    for (int j = i + 1; j < trenutniSkup.size(); j++){
                        //System.out.printf("ispitujem: %s i %s\n", trenutniSkup.get(i), trenutniSkup.get(j));

                        //petlja za provjeravanje istovjetnosti stanja
                        for (String simbol : simboli){

                            String s1 = "";
                            String s2 = "";

                            //iteriranje po funkcijama
                            for (Vector<String> vc : prijelazi){
                                if (vc.get(0).equals(trenutniSkup.get(i)) &&
                                        vc.get(1).equals(simbol)){
                                            s1 = vc.get(2);
                                }

                                if (vc.get(0).equals(trenutniSkup.get(j)) &&
                                        vc.get(1).equals(simbol)){
                                            s2 = vc.get(2);
                                }

                            }

                            //provjeravanje jesu li stanja u istoj grupi
                            boolean check = false;
                            for (List<String> l : lista_listi_pom){
                                if (l.contains(s1) && l.contains(s2)){
                                    check = true; //u istoj su grupi, idi na sljedeci simbol
                                    break;
                                }
                            }

                            //nisu u istoj grupi, drugi clan razdvoji
                            if ( (!check) && (!novaLista.contains(trenutniSkup.get(j))) ){
                                novaLista.add(trenutniSkup.get(j));
                                //System.out.printf("Dodao %s u novu\n", trenutniSkup.get(j));
                                provjera = true;
                                break;
                            }

                        }//zavrsetak simbol petlje

                    }//zavrsetak j petlje

                }//zavrsetak i i kompletne i-j petlje

                //uklanjanje clanova
                List<String> toBeRemoved = new ArrayList<>();
                for (String s : trenutniSkup) {
                    if (novaLista.contains(s)) {
                        toBeRemoved.add(s);
                    }
                }
                int a = lista_listi.indexOf(trenutniSkup);
                if (!toBeRemoved.isEmpty()){
                    lista_listi.get(a).removeAll(toBeRemoved);
                }


                //dodavanje nove liste ako nije prazna
                if (!novaLista.isEmpty()){
                    lista_listi.add(novaLista);
                }

            }//zavrsetak petlje trenutnog skupa
        }



        //ponovno slaganje funkcija
        List<Vector<String>> pom_funkc = new ArrayList<>();

        pom_funkc.addAll(prijelazi);

        for (Vector<String> vc : pom_funkc){

            for (List<String> lista : lista_listi){
                if (lista.contains(vc.get(0))){
                    if (lista.get(0).equals(vc.get(0))){
                        for (List<String> l : lista_listi){
                            if (l.contains(vc.get(2))){
                                if (l.indexOf(vc.get(2)) > 0){
                                    //vc.remove(2);
                                    //vc.add(l.get(0));
                                    int a = prijelazi.indexOf(vc);
                                    prijelazi.get(a).remove(2);
                                    prijelazi.get(a).add(l.get(0));
                                }
                            }
                        }
                    } else {
                        int a = prijelazi.indexOf(vc);
                        prijelazi.remove(a);
                    }
                }
            }
        }

        //printanje minimiziranih stanja
        listaStanja.clear();
        for (List<String> l : lista_listi){
            listaStanja.add(l.get(0));
        }

        Collections.sort(listaStanja);
        for (int i = 0; i < listaStanja.size(); i++){
            if (i == 0){
                System.out.printf("%s", listaStanja.get(i));
            } else{
                System.out.printf(",%s", listaStanja.get(i));
            }
        }
        System.out.printf("\n");

        //printanje abecede
        for (int i = 0; i < simboli.size(); i++){
            if (i == 0){
                System.out.printf("%s", simboli.get(0));
            } else{
                System.out.printf(",%s", simboli.get(i));
            }
        }
        System.out.printf("\n");

        //System.out.println("printam probu prihv  2");
        //System.out.println(prihvStanja);
        //printanje novih prihvatljivih stanja
        boolean prov = true;
       for (int i = 0; i < prihvStanja.size(); i++){
           if (listaStanja.contains(prihvStanja.get(i))){
               if (prov){
                   prov = false;
                   System.out.printf("%s", prihvStanja.get(i));
               } else {
                   System.out.printf(",%s", prihvStanja.get(i));
               }
           }
       }
        System.out.printf("\n");

        //printanje pocetnog stanja
        for(List<String> l : lista_listi){
            if (l.contains(pocetnoStanje)){
                pocetnoStanje = l.get(0);
            }
        }
        System.out.println(pocetnoStanje);

        //printanje novih fja prijelaza
        for (Vector<String> vec : prijelazi){
            System.out.printf("%s,%s->%s\n",vec.get(0), vec.get(1), vec.get(2));
        }






    }
}
