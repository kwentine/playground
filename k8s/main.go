package main

import (
	"flag"
	"fmt"
	"os"
	"path"
	"k8s.io/client-go/tools/clientcmd"
)

func main() {
	var kubeconfig *string
	home, err := os.UserHomeDir()
	if err != nil {
		panic("Failed to resolve home dir")
	}
	kubeconfig = flag.String("kubeconfig", path.Join(home, ".kube", "config"), "Kubeconfig")
	flag.Parse()
	fmt.Println("Kubernetes client examples")
	fmt.Println("Kubeconfig:", *kubeconfig)
	config, err := clientcmd.BuildConfigFromFlags("", *kubeconfig)
	if err != nil {
		panic(err)
	}
	fmt.Println(config)
}
